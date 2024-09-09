#include "GLRenderEngine.h"
#include "Funcs.h"
#include "GLHeaders.h"
#include "GLShaderProgram.h"
#include "GLShaderProgramLoader.h"
#include "GLVertexBuffer.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderModel.h"
#include "RGBAColor.h"
#include "SomeGLRenderCommand.h"
#include "SomeRenderCommandModel.h"
#include "SomeRenderContext.h"
#include "Utils.h"
#include "Vector3.h"
#include <type_traits>

using namespace PJ;
using namespace std;

void GLRenderEngine::EnableVertexAttributeArray(GLuint location, bool isEnabled) {
    Base::EnableVertexAttributeArray(location, isEnabled);

    // NOTE: problem about checking state changes is that the vertex
    // attrib value might be difficult from program to program.
    if (isEnabled) {
        glEnableVertexAttribArray(location);
    } else {
        glDisableVertexAttribArray(location);
    }
}

void GLRenderEngine::VertexAttributePointer(
    GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
    const GLvoid* pointer
) {
    RunGL(
        [=]() { glVertexAttribPointer(location, size, type, normalized, stride, pointer); },
        "glVertexAttribPointer"
    );
}

void GLRenderEngine::DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) {
    glDrawArrays(drawMode, drawFirst, drawCount);
}

void GLRenderEngine::DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    glDrawElements(mode, count, type, indices);
}

void GLRenderEngine::Use(GLShaderProgram& program) {
    RunGL([&]() { glUseProgram(program.GLId()); }, "glUseProgram: " + program.id);

    // Enable attributes for vertex shader
    /*
        IMPORTANT: Permissive enabling works fine on Mac/OS, but crashes on
       Windows. Windows OpenGL requires that only the correct arrays are enabled
       for the shader.
     */
    OrderedSet<GLuint> activeAttributeLocations;
    for (auto& keyValue : program.attributeLocations) {
        auto location = keyValue.second;
        activeAttributeLocations.insert(location);
    }
    EnableOnlyVertexAttributeArrays(activeAttributeLocations);
}

void GLRenderEngine::UniformMatrix4fv(GLint location, const GLfloat* value) {
    Base::UniformMatrix4fv(location, value);

    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void GLRenderEngine::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    Base::SetViewport(x, y, width, height);

    glViewport(x, y, width, height);
}

std::optional<GLenum> GLRenderEngine::FeatureIdToGLFeatureId(String featureId) {
    if (featureIdToGLFeatureIdMap.find(featureId) == featureIdToGLFeatureIdMap.end()) {
        return std::nullopt;
    }

    return std::make_optional(featureIdToGLFeatureIdMap[featureId]);
}

void GLRenderEngine::EnableFeature(String featureId, bool isEnabled) {
    Base::EnableFeature(featureId, isEnabled);

    auto glFeatureId = FeatureIdToGLFeatureId(featureId);
    if (!glFeatureId) {
        PJLog("ERROR. Unsupported feature %s", featureId.c_str());
        return;
    }

    if (isEnabled) {
        glEnable(glFeatureId.value());
    } else {
        glDisable(glFeatureId.value());
    }
}

void GLRenderEngine::SetLineWidth(float lineWidth) {
    Base::SetLineWidth(lineWidth);

    glLineWidth(lineWidth);
}

void GLRenderEngine::BindFrameBuffer(GLuint fb) {
    Base::BindFrameBuffer(fb);

    glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void GLRenderEngine::RunGL(std::function<void()> command, String name) {
    GUARD(command)
    command();

    while (auto error = glGetError()) {
        PJLog("ERROR: %s: %d", name.c_str(), error);
    }
}

void GLRenderEngine::GoInternal() {
    Base::GoInternal();

    const char* gl_version = (const char*)glGetString(GL_VERSION);
    PJLog("OpenGL Version: %s", gl_version);

    /*
     A VAO (Vertex array object) stores pointers to VBOs (Vertex buffer objects)
     and is used by the GPU to store state. You can swap VAOs if you want to
     swap bound VBO state.
     */
    RunGL([this]() { glGenVertexArrays(1, &vao); }, "glGenVertexArrays");
    BindVertexArray(vao);

    // Load the shader programs from the info registry
    for (auto& info : GLShaderProgram::Info::registry) {
        GLShaderProgramLoader loader;
        auto program = loader.LoadFromShaderPaths(info.vertexShaderPath, info.fragmentShaderPath);
        if (!program) {
            continue;
        }

        program->id = info.id;
        SomeShaderProgram::registry[info.id] = program;
    }

    SetBlendMode(GLBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // https://learnopengl.com/Advanced-OpenGL/Face-culling
    // Our mesh code is shared with Unity, which uses Clockwise vertex ordering
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Enable 2D textures
    glEnable(GL_TEXTURE_2D);

    // Enable Z-ordering via depth testing
    glEnable(GL_DEPTH_TEST);

    // TODO: temp code
    glEnable(GL_MULTISAMPLE);

    ScanGLExtensions();

    featureIdToGLFeatureIdMap[RenderFeature::Blend] = GL_BLEND;
    featureIdToGLFeatureIdMap[RenderFeature::ScissorTest] = GL_SCISSOR_TEST;
}

void GLRenderEngine::SetBlendMode(GLBlendMode blendMode) {
    Base::SetBlendMode(blendMode);

    RunGL([&]() { glBlendFunc(blendMode.sFactor, blendMode.dFactor); }, "glBlendFunc");
}

void GLRenderEngine::BindVertexArray(GLuint vao) {
    Base::BindVertexArray(vao);

    RunGL([=]() { glBindVertexArray(vao); }, "glBindVertexArray");
}

SP<GLVertexBuffer> GLRenderEngine::BuildVertexBuffer(GLVertexBufferPlan const& plan) {
    GLsizei totalSize = 0;
    for (auto& item : plan.items) {
        uint32_t itemSize = item.Size();
        totalSize += itemSize;
    }

    if (0 == totalSize) {
        return nullptr;
    }

    GLuint VBO;
    RunGL([&]() { glGenBuffers(1, &VBO); }, "Gen VBO");
    RunGL([&]() { glBindBuffer(GL_ARRAY_BUFFER, VBO); }, "Bind VBO");

    int offset = 0;

    RunGL([&]() { glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW); }, "VBO Data");

    auto result = MAKE<GLVertexBuffer>(VBO);

    for (auto& item : plan.items) {
        auto itemSize = item.Size();
        RunGL(
            [&]() { glBufferSubData(GL_ARRAY_BUFFER, offset, itemSize, item.data->Pointer()); },
            "VBO Data"
        );

        result->attributes[item.attributeId].offset = offset;
        result->attributes[item.attributeId].glType = item.glType;
        result->attributes[item.attributeId].normalize = item.normalize;

        offset += itemSize;
    }

    return result;
}

SP<GLIndexBuffer> GLRenderEngine::BuildIndexBuffer(VectorList<uint32_t> indices) {
    GLuint IBO;
    RunGL([&]() { glGenBuffers(1, &IBO); }, "Gen IBO");
    RunGL([&]() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); }, "Bind IBO");
    RunGL(
        [&]() {
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(),
                GL_STATIC_DRAW
            );
        },
        "IBO Data"
    );

    return MAKE<GLIndexBuffer>(IBO);
}

void GLRenderEngine::BindVertexBuffer(GLuint vbo) {
    Base::BindVertexBuffer(vbo);

    RunGL([=]() { glBindBuffer(GL_ARRAY_BUFFER, vbo); }, "Bind VBO");
}

void GLRenderEngine::BindIndexBuffer(GLuint ibo) {
    Base::BindIndexBuffer(ibo);

    RunGL([=]() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); }, "Bind IBO");
}

SP<SomeGLRenderCommand> GLRenderEngine::BuildRenderCommand(SomeRenderCommandModel& proxyCommand) {
    // FUTURE: replace with a map and factories
    if (proxyCommand.id == RenderCommandId::ProjectionMatrixLoadOrthographic) {
        auto command = As<RenderCommandModel<Vector2>>(&proxyCommand);
        GUARDR(command, nullptr)
        return GLRenderCommands::ProjectionMatrixLoadOrthographic(command->core);
    } else if (proxyCommand.id == RenderCommandId::LoadTranslate) {
        auto command = As<RenderCommandModel<Vector3>>(&proxyCommand);
        GUARDR(command, nullptr)
        return GLRenderCommands::LoadTranslate(command->core);
    }

    return nullptr;
}

void GLRenderEngine::ProjectionMatrixLoadOrthographic(Vector2 size) {
    // TODO: are these 1, -1 values correct? Should we use values from the camera?
    // TODO: should we be using glOrtho/glMatrixMode instead?
    projectionMatrix.LoadOrthographic(0, size.x, 0, size.y, 1, -1);
}

void GLRenderEngine::LoadTranslate(Vector3 value) {
    viewMatrix.LoadTranslate(value);
}

void GLRenderEngine::RenderStart(RenderContextModel& contextModel) {
    renderPlans.clear();

    //    auto& proxyCommands = contextModel.phasedProxyCommands;
    //    auto thisPhaseProxyCommands = proxyCommands.find(RenderPhase::Start);
    //    if (thisPhaseProxyCommands != proxyCommands.end() &&
    //    !IsEmpty(thisPhaseProxyCommands->second)) {
    //        for (auto& proxyCommandI : thisPhaseProxyCommands->second) {
    //            auto& proxyCommandUP = proxyCommandI;
    //
    //            if (auto renderCommand = BuildRenderCommand(*proxyCommandUP)) {
    //                renderCommand->Run(*this);
    //            }
    //        }
    //    } else {
    //        // If no projection commands are specified, use orthographic-cartesian
    //        // projection
    //        projectionMatrix.LoadOrthographic(
    //            0, renderState.viewport.width, 0, renderState.viewport.height, 1, -1
    //        );
    //        viewMatrix.LoadTranslate(
    //            Vector3(renderState.viewport.width / 2.0f, renderState.viewport.height / 2.0f, 0)
    //        );
    //    }
}

void GLRenderEngine::RenderProcess(RenderDrawModel const& drawModel) {
    for (auto& renderModel : drawModel.models) {
#ifdef PROFILE
        DevProfiler devProfilerRenderProcess("Render- Process", [](String value) {
            cout << value;
        });
#endif
        RenderProcess(renderModel);
    }
}

void GLRenderEngine::RenderProcess(RenderModel const& model) {
    auto modelMaterial = model.Material();

    GUARD(modelMaterial)
    auto glProgram = As<GLShaderProgram>(modelMaterial->ShaderProgram().get());
    if (nullptr == glProgram) {
        PJLog("ERROR. GLShaderProgram required");
        return;
    }

    VectorList<Vector3> const& vertices = model.Vertices();
    VectorList<Vector2> const& uvs = model.UVs();
    auto vertexCount = vertices.size();

    VectorList<Color> colors_float(vertexCount);
    VectorList<RGBAColor> colors_byte(vertexCount);
    VectorList<Vector2> texCoords(vertexCount);

    // First pass of this is very inefficient. We'll create an IBO, VBO, etc.
    // for each render pass. FUTURE: optimize as needed
    GLVertexBufferPlan vboPlan;
    vboPlan.Add("a_position", vertices);

    auto& modelColors = model.Colors();

    // For now we'll take the easy route and use float colors
    // FUTURE: evaluate converting colors to 32 bit single values for efficiency
    auto hasColorAttribute = glProgram->HasVertexAttribute("a_color");
    if (hasColorAttribute) {
        auto isColorsEmpty = modelColors.size() == 0;
        for (int i = 0; i < vertices.size(); i++) {
            auto color = Color::white;
            if (!isColorsEmpty) {
                auto colorIndex = i % modelColors.size();
                color = modelColors[colorIndex];
            }

            switch (colorFormat) {
            case ColorFormat::Float:
                colors_float[i] = color;
                break;
            case ColorFormat::Byte:
                colors_byte[i] = (RGBAColor)color;
                break;
            }
        }

        switch (colorFormat) {
        case ColorFormat::Float:
            vboPlan.Add("a_color", colors_float);
            break;
        case ColorFormat::Byte:
            vboPlan.Add("a_color", colors_byte);
            break;
        }
    }

    auto hasTextureCoordAttribute = glProgram->HasVertexAttribute("a_texCoord");
    if (hasTextureCoordAttribute) {
        if (uvs.size() != vertices.size()) {
            PJLog("ERROR. Trying to render a texture shader with invalid uvs.");
            return;
        }

        texCoords = uvs;

        vboPlan.Add("a_texCoord", texCoords);
    }

    auto renderPlan = MAKE<GLRenderPlan>(model, vboPlan);
    Add(renderPlans, renderPlan);
}

void GLRenderEngine::RenderDraw(RenderDrawModel const& drawModel) {
    RenderProcess(drawModel);

    VectorList<SP<GLRenderPlan>> noBlendRenderPlans;
    std::copy_if(
        renderPlans.begin(), renderPlans.end(), std::back_inserter(noBlendRenderPlans),
        [&](SP<GLRenderPlan> const& plan) {
            return !plan->model.IsFeatureEnabled(RenderFeature::Blend);
        }
    );

    VectorList<SP<GLRenderPlan>> blendRenderPlans;
    std::copy_if(
        renderPlans.begin(), renderPlans.end(), std::back_inserter(blendRenderPlans),
        [&](SP<GLRenderPlan> const& plan) {
            return plan->model.IsFeatureEnabled(RenderFeature::Blend);
        }
    );

    // Sort blended back-to-front
    std::sort(
        blendRenderPlans.begin(), blendRenderPlans.end(),
        [](SP<GLRenderPlan> const& lhs, SP<GLRenderPlan> const& rhs) {
            return lhs->model.z > rhs->model.z;
        }
    );

    // Sort opaque front-to-back
    std::sort(
        noBlendRenderPlans.begin(), noBlendRenderPlans.end(),
        [](SP<GLRenderPlan> const& lhs, SP<GLRenderPlan> const& rhs) {
            return lhs->model.z < rhs->model.z;
        }
    );

    glEnable(GL_DEPTH_TEST);
    RenderDrawPlans(noBlendRenderPlans);

    glDisable(GL_DEPTH_TEST);
    // TODO: do we need glDepthFunc here?
    RenderDrawPlans(blendRenderPlans);

    // FUTURE: support batching, logging
    renderPlans.clear();
}

void GLRenderEngine::RenderDrawPlans(VectorList<SP<GLRenderPlan>> const& renderPlans) {
    for (auto& rp : renderPlans) {
        auto vboPlan = rp->vboPlan;
        auto model = rp->model;
        auto modelMaterial = model.Material();
        GUARD_CONTINUE(modelMaterial)

        auto glProgram = As<GLShaderProgram>(modelMaterial->ShaderProgram().get());
        GUARD(glProgram);

        auto modelMatrix = model.matrix;

        auto vbo = BuildVertexBuffer(vboPlan);
        GUARD(vbo)

        auto ibo = BuildIndexBuffer(model.Indices());
        GUARD(ibo)

        auto hasColorAttribute = glProgram->HasVertexAttribute("a_color");
        auto hasTextureCoordAttribute = glProgram->HasVertexAttribute("a_texCoord");

        // MARK: - Uniforms

        Use(*glProgram);

        for (auto& texture : model.textureModels) {
            // ? glActiveTexture is causing VBO errors. Investigate
            //        glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texture.renderId);
        }
        //    glActiveTexture(0);

        if (glProgram->uniformLocations.find("u_mvpMatrix") != glProgram->uniformLocations.end()) {
            Matrix4x4 viewProjectionMatrix = projectionMatrix * viewMatrix;
            Matrix4x4 modelViewProjection = viewProjectionMatrix * modelMatrix;

            auto location = glProgram->uniformLocations["u_mvpMatrix"];
            UniformMatrix4fv(location, modelViewProjection.data());
        }

        // TODO: These can be optimized by not using the has check, or has returns something
        if (glProgram->HasUniform("u_float")) {
            float value = 1.0f;
            if (modelMaterial->UniformFloats().size() > 0) {
                value = modelMaterial->UniformFloats()[0];
            }

            glUniform1f(glProgram->uniformLocations["u_float"], value);
        }

        if (glProgram->HasUniform("u_color")) {
            Color color = Color::white;
            if (modelMaterial->UniformColors().size() > 0) {
                color = modelMaterial->UniformColors()[0];
            }
            glUniform4f(glProgram->uniformLocations["u_color"], color.r, color.g, color.b, color.a);
        }

        BindVertexBuffer(vbo->glId);
        BindIndexBuffer(ibo->glId);

        VertexAttributePointer(
            glProgram->attributeLocations["a_position"], 3, GL_FLOAT, false, 0, 0
        );

        if (hasColorAttribute) {
            auto attribute = vbo->attributes["a_color"];

            VertexAttributePointer(
                glProgram->attributeLocations["a_color"], 4, attribute.glType, attribute.normalize,
                0, (void*)(uintptr_t)attribute.offset
            );
        }

        if (hasTextureCoordAttribute) {
            VertexAttributePointer(
                glProgram->attributeLocations["a_texCoord"], 2, GL_FLOAT, false, 0,
                (void*)(uintptr_t)vbo->attributes["a_texCoord"].offset
            );
        }

        for (auto& i : modelMaterial->Features()) {
            auto key = i.first;
            auto status = i.second;

            switch (status) {
            case RenderFeatureState::Enable:
                EnableFeature(key, true);
                break;
            case RenderFeatureState::Disable:
                EnableFeature(key, false);
                break;
            }
        }

        DrawElements(GL_TRIANGLES, (GLsizei)model.Indices().size(), GL_UNSIGNED_INT, nullptr);
    }
}

void GLRenderEngine::ScanGLExtensions() {
    OrderedSet<String> glExtensions;

    GLint count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);

    ForCount(0, count, [&](int i) {
        auto extensionId = (char*)glGetStringi(GL_EXTENSIONS, i);
        glExtensions.insert(String(extensionId));
    });

    PJLog("****** OPENGL EXTENSIONS ******");
    for (auto& extension : glExtensions) {
        PJLog("%s", extension.c_str());
    }
    PJLog("******                   ******");
}
