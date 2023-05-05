#include "GLRenderEngine.h"
#include "GLHeaders.h"
#include "GLShaderProgram.h"
#include "GLVertexBuffer.h"
#include "GLShaderProgramLoader.h"
#include "Vector3.h"
#include "RenderFeatures.h"
#include "RenderModel.h"
#include "Color32.h"

using namespace PJ;
using namespace std;

void GLRenderEngine::EnableVertexAttributeArray(GLuint location, bool isEnabled)
{
    Base::EnableVertexAttributeArray(location, isEnabled);

    // NOTE: problem about checking state changes is that the vertex
    // attrib value might be difficult from program to program.
    if (isEnabled) {
        glEnableVertexAttribArray(location);
    }
    else {
        glDisableVertexAttribArray(location);
    }
}

void GLRenderEngine::VertexAttributePointer(GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    RunGL([=] () { glVertexAttribPointer(location, size, type, normalized, stride, pointer); }, "glVertexAttribPointer");
}

void GLRenderEngine::DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) {
    glDrawArrays(drawMode, drawFirst, drawCount);
}

void GLRenderEngine::DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    glDrawElements(mode, count, type, indices);
}

void GLRenderEngine::Use(GLShaderProgram& program) {
    RunGL([&] () { glUseProgram(program.GLId()); }, "glUseProgram: " + program.id);

    // Enable attributes for vertex shader
    /*
        IMPORTANT: Permissive enabling works fine on Mac/OS, but crashes on Windows.
        Windows OpenGL requires that only the correct arrays are enabled for the shader.
     */
    Set<GLuint> activeAttributeLocations;
    for (auto keyValue : program.attributeLocations) {
        auto location = keyValue.second;
        activeAttributeLocations.Add(location);
    }
    EnableOnlyVertexAttributeArrays(activeAttributeLocations);
}

void GLRenderEngine::UniformMatrix4fv(GLint location, const GLfloat* value)
{
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
    }
    else {
        glDisable(glFeatureId.value());
    }
}

void GLRenderEngine::SetLineWidth(float lineWidth) {
    Base::SetLineWidth(lineWidth);

    glLineWidth(lineWidth);
}

void GLRenderEngine::BindFrameBuffer(GLuint fb)
{
    Base::BindFrameBuffer(fb);

    glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void GLRenderEngine::RunGL(std::function<void()> command, String name) {
    command();

    while (auto error = glGetError()) {
        PJLog("ERROR: %s: %d", name.c_str(), error);
    }
}

void GLRenderEngine::GoInternal()
{
    Base::GoInternal();

    const char* gl_version = (const char*)glGetString(GL_VERSION);
    PJLog("OpenGL Version: %s", gl_version);

    /*
     A VAO (Vertex array object) stores pointers to VBOs (Vertex buffer objects) and is used
     by the GPU to store state. You can swap VAOs if you want to swap bound VBO state.
     */
    RunGL([this] () { glGenVertexArrays(1, &vao); }, "glGenVertexArrays");
    BindVertexArray(vao);

    // Load the shader programs from the info registry
    for (auto info : GLShaderProgram::Info::registry) {
        GLShaderProgramLoader loader;
        auto program = loader.LoadFromShaderPaths(info.vertexShaderPath, info.fragmentShaderPath);
        if (!program) {
            continue;
        }

        program->id = info.id;
        GLShaderProgram::registry[info.id] = program;
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

    featureIdToGLFeatureIdMap[RenderFeatures::Blend] = GL_BLEND;
    featureIdToGLFeatureIdMap[RenderFeatures::ScissorTest] = GL_SCISSOR_TEST;
}

void GLRenderEngine::SetBlendMode(GLBlendMode blendMode) {
    Base::SetBlendMode(blendMode);

    RunGL([&] () { glBlendFunc(blendMode.sFactor, blendMode.dFactor); }, "glBlendFunc");
}

void GLRenderEngine::BindVertexArray(GLuint vao) {
    Base::BindVertexArray(vao);

    RunGL([=] () { glBindVertexArray(vao); }, "glBindVertexArray");
}

SP<GLVertexBuffer> GLRenderEngine::BuildVertexBuffer(GLVertexBufferPlan const& plan)
{
    GLsizei totalSize = 0;
    for (auto item : plan.items) {
        uint32_t itemSize = item.Size();
        totalSize += itemSize;
    }

    if (0 == totalSize) {
        return nullptr;
    }

    GLuint VBO;
    RunGL([&] () { glGenBuffers(1, &VBO); }, "Gen VBO");
    RunGL([&] () { glBindBuffer(GL_ARRAY_BUFFER, VBO); }, "Bind VBO");

    int offset = 0;

    RunGL([&] () { glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW); }, "VBO Data");

    auto result = MAKE<GLVertexBuffer>(VBO);

    for (auto item : plan.items) {
        auto itemSize = item.Size();
        RunGL([&] () { glBufferSubData(GL_ARRAY_BUFFER, offset, itemSize, item.data); }, "VBO Data");

        result->attributes[item.attributeId].offset = offset;
        result->attributes[item.attributeId].glType = item.glType;
        result->attributes[item.attributeId].normalize = item.normalize;

        offset += itemSize;
    }

    return result;
}

SP<GLIndexBuffer> GLRenderEngine::BuildIndexBuffer(VectorList<uint32_t> indices) {
    GLuint IBO;
    RunGL([&] () { glGenBuffers(1, &IBO); }, "Gen IBO");
    RunGL([&] () { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); }, "Bind IBO");
    RunGL([&] () { glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW); }, "IBO Data");

    return MAKE<GLIndexBuffer>(IBO);
}

void GLRenderEngine::BindVertexBuffer(GLuint vbo)
{
    Base::BindVertexBuffer(vbo);

    RunGL([=] () { glBindBuffer(GL_ARRAY_BUFFER, vbo); }, "Bind VBO");
}

void GLRenderEngine::BindIndexBuffer(GLuint ibo)
{
    Base::BindIndexBuffer(ibo);
    
    RunGL([=] () { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); }, "Bind IBO");
}

void GLRenderEngine::RenderStart() {
    renderPlans.clear();

    projectionMatrix.LoadOrthographic(0, renderState.viewport.width, 0, renderState.viewport.height, 1, -1);
    viewMatrix.LoadTranslate(Vector3(renderState.viewport.width / 2.0f, renderState.viewport.height / 2.0f, 0));
}

void GLRenderEngine::RenderProcess(RenderModel const& model)  {
    auto glProgram = dynamic_cast<GLShaderProgram*>(&model.shaderProgram);
    if (nullptr == glProgram) {
        PJLog("ERROR. GLShaderProgram required");
        return;
    }

    Use(*glProgram);

    auto modelMatrix = model.matrix;

    VectorList<Vector3> vertices = model.vertices;
    auto vertexCount = model.vertices.size();

    VectorList<Color> colors_float(vertexCount);
    VectorList<Color32> colors_byte(vertexCount);
    VectorList<Vector2> texCoords(vertexCount);

    for (int i = 0; i < model.textures.size(); i++) {
        auto & texture = model.textures[i];

        // ? glActiveTexture is causing VBO errors. Investigate
//        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture->renderId);
    }
//    glActiveTexture(0);

    // First pass of this is very inefficient. We'll create an IBO, VBO, etc. for each render pass.
    // FUTURE: optimize as needed
    GLVertexBufferPlan vboPlan;
    vboPlan.Add("a_position", model.vertices);

    // For now we'll take the easy route and use float colors
    // FUTURE: evaluate converting colors to 32 bit single values for efficiency
    auto hasColorAttribute = glProgram->HasVertexAttribute("a_color");
    if (hasColorAttribute) {
        auto isColorsEmpty = model.colors.size() == 0;
        for (int i = 0; i < model.vertices.size(); i++) {
            auto color = Color::white;
            if (!isColorsEmpty) {
                auto colorIndex = i % model.colors.size();
                color = model.colors[colorIndex];
            }

            switch (colorFormat) {
                case ColorFormat::Float:
                    colors_float[i] = color;
                    break;
                case ColorFormat::Byte:
                    colors_byte[i] = (Color32)color;
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
        if (model.uvs.size() != model.vertices.size()) {
            PJLog("ERROR. Trying to render a texture shader with invalid uvs.");
            return;
        }

        texCoords = model.uvs;
        vboPlan.Add("a_texCoord", texCoords);
    }

    auto renderPlan = GLRenderPlan(model, vboPlan);
    renderPlans.Add(renderPlan);
    
    auto vbo = BuildVertexBuffer(vboPlan);
    auto ibo = BuildIndexBuffer(model.indices);

    // MARK: - Uniforms

    if (glProgram->uniformLocations.find("u_mvpMatrix") != glProgram->uniformLocations.end()) {
        Matrix4x4 viewProjectionMatrix = projectionMatrix * viewMatrix;
        Matrix4x4 modelViewProjection = viewProjectionMatrix * modelMatrix;

        auto location = glProgram->uniformLocations["u_mvpMatrix"];
        UniformMatrix4fv(location, modelViewProjection.data());
    }

    if (glProgram->uniformLocations.find("u_float") != glProgram->uniformLocations.end()) {
        float value = 1.0f;
        if (model.uniformFloats.size() > 0) {
            value = model.uniformFloats[0];
        }

        glUniform1f(glProgram->uniformLocations["u_float"], value);
    }

    if (glProgram->HasUniform("u_color")) {
        Color color = Color::white;
        if (model.uniformColors.size() > 0) {
            color = model.uniformColors[0];
        }
        glUniform4f(glProgram->uniformLocations["u_color"], color.r, color.g, color.b, color.a);
    }

    BindVertexBuffer(vbo->glId);
    BindIndexBuffer(ibo->glId);

    VertexAttributePointer(glProgram->attributeLocations["a_position"], 3, GL_FLOAT, false, 0, 0);

    if (hasColorAttribute) {
        auto attribute = vbo->attributes["a_color"];

        VertexAttributePointer(glProgram->attributeLocations["a_color"], 4, attribute.glType, attribute.normalize, 0, (void*)(uintptr_t)attribute.offset);
    }

    if (hasTextureCoordAttribute) {
        VertexAttributePointer(glProgram->attributeLocations["a_texCoord"], 2, GL_FLOAT, false, 0, (void*)(uintptr_t)vbo->attributes["a_texCoord"].offset);
    }

    for (auto i : model.features) {
        auto key = i.first;
        auto status = i.second;

        switch (status) {
            case RenderFeatureStatus::Enable:
                EnableFeature(key, true);
                break;
            case RenderFeatureStatus::Disable:
                EnableFeature(key, false);
                break;
        }
    }

    DrawElements(GL_TRIANGLES, (GLsizei)model.indices.size(), GL_UNSIGNED_INT, nullptr);
}

void GLRenderEngine::RenderDraw() {
    // FUTURE: support batching, logging
    renderPlans.clear();
}

void GLRenderEngine::ScanGLExtensions()
{
    Set<String> glExtensions;

    GLint count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);

    for (int i = 0; i < count; i++) {
        auto extensionId = (char*)glGetStringi(GL_EXTENSIONS, i);
        glExtensions.Add(String(extensionId));
    }

    PJLog("****** OPENGL EXTENSIONS ******");
    for (auto& extension : glExtensions) {
        PJLog("%s", extension.c_str());
    }
    PJLog("******                   ******");
}
