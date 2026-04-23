#include "GLRenderEngine.h"
#include "Funcs.h"
#include "GLHeaders.h"
#include "GLLoadShaderProgramOperation.h"
#include "GLShaderProgram.h"
#include "GLSomeBuffer.h"
#include "GLTextureBuffer.h"
#include "MaterialRenderModel.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderPassModel.h"
#include "RGBAColor.h"
#include "SomeGLRenderCommand.h"
#include "SomeRenderCommandModel.h"
#include "SomeRenderContext.h"
#include "StencilPopRenderModel.h"
#include "StencilPushRenderModel.h"
#include "Utils.h"
#include "Vector3.h"
#include "ViewportRenderModel.h"
#include <type_traits>

using namespace PJ;
using namespace std;

void GLRenderEngine::EnableVertexAttributeArray(GLuint location, bool isEnabled) {
    Base::EnableVertexAttributeArray(location, isEnabled);

    // NOTE: problem about checking state changes is that the vertex
    // attrib value might be different from program to program.
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
     IMPORTANT: Permissive enabling works fine on Mac OS, but crashes on
     Windows. Windows OpenGL requires that only the correct arrays are enabled
     for the shader.
     */
    UnorderedSet<GLuint> activeAttributeLocations;
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
    auto i = featureIdToGLFeatureIdMap.find(featureId);
    if (i == featureIdToGLFeatureIdMap.end()) {
        return {};
    }
    return i->second;
}

void GLRenderEngine::EnableFeature(String featureId, bool isEnabled) {
    if (optimizeStateSwitches) {
        GUARD(renderState.IsFeatureEnabled(featureId) != isEnabled);
    }
    Base::EnableFeature(featureId, isEnabled);

    auto glFeatureId = FeatureIdToGLFeatureId(featureId);
    if (!glFeatureId) {
        PJ::Log("ERROR. Unsupported feature: ", featureId.c_str());
        return;
    }

    if (isEnabled) {
        glEnable(glFeatureId.value());
    } else {
        glDisable(glFeatureId.value());
    }
}

void GLRenderEngine::BindFrameBuffer(GLuint id) {
    Base::BindFrameBuffer(id);

    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void GLRenderEngine::BindTexture2D(GLuint id) {
    glBindTexture(GL_TEXTURE_2D, id);
}

void GLRenderEngine::BindRenderBuffer(GLenum target, GLuint id) {
    glBindRenderbuffer(target, id);
}

void GLRenderEngine::RunGL(std::function<void()> command, String name) {
    GUARD(command)
    command();

    while (auto error = glGetError()) {
        PJ::Log(std::format("ERROR: {}, id: {}", name, error));
    }
}

void GLRenderEngine::OnGo() {
    Base::OnGo();

    const char* gl_version = (const char*)glGetString(GL_VERSION);
    PJ::Log("OpenGL Version: ", gl_version);

    /*
     A VAO (Vertex array object) stores pointers to VBOs (Vertex buffer objects)
     and is used by the GPU to store state. You can swap VAOs if you want to
     swap bound VBO state.
     */
    RunGL([this]() { glGenVertexArrays(1, &vao); }, "glGenVertexArrays");
    BindVertexArray(vao);

    SetBlendMode(GLBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    featureIdToGLFeatureIdMap[RenderFeature::Blend] = GL_BLEND;
    featureIdToGLFeatureIdMap[RenderFeature::CullFace] = GL_CULL_FACE;
    featureIdToGLFeatureIdMap[RenderFeature::Texture2D] = GL_TEXTURE_2D;
    featureIdToGLFeatureIdMap[RenderFeature::DepthTest] = GL_DEPTH_TEST;
    featureIdToGLFeatureIdMap[RenderFeature::StencilTest] = GL_STENCIL_TEST;
    featureIdToGLFeatureIdMap[RenderFeature::MultiSample] = GL_MULTISAMPLE;

    // https://learnopengl.com/Advanced-OpenGL/Face-culling
    // Our mesh code matches Unity, which uses Clockwise vertex ordering
    EnableFeature(RenderFeature::CullFace, true);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    EnableFeature(RenderFeature::Texture2D, true);
    EnableFeature(RenderFeature::DepthTest, true);
    EnableFeature(RenderFeature::StencilTest, true);

    EnableFeature(RenderFeature::MultiSample, true);

    ScanGLExtensions();
}

void GLRenderEngine::SetBlendMode(GLBlendMode blendMode) {
    Base::SetBlendMode(blendMode);

    RunGL([&]() { glBlendFunc(blendMode.sourceFactor, blendMode.destFactor); }, "glBlendFunc");
}

void GLRenderEngine::BindVertexArray(GLuint vao) {
    Base::BindVertexArray(vao);

    RunGL([=]() { glBindVertexArray(vao); }, "glBindVertexArray");
}

UP<GLVertexBuffer> GLRenderEngine::BuildVertexBuffer(GLVertexBufferPlan const& plan) {
    GLsizei totalSize = 0;
    for (auto& item : plan.items) {
        uint32_t itemSize = item->Size();
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

    auto result = NEW<GLVertexBuffer>(VBO);

    for (auto& item : plan.items) {
        auto itemSize = item->Size();
        RunGL(
            [&]() { glBufferSubData(GL_ARRAY_BUFFER, offset, itemSize, item->dataPtr); }, "VBO Data"
        );

        result->attributes[item->attributeId].offset = offset;
        result->attributes[item->attributeId].glType = item->glType;
        result->attributes[item->attributeId].normalize = item->normalize;

        offset += itemSize;
    }

    return result;
}

UP<GLIndexBuffer> GLRenderEngine::BuildIndexBuffer(std::span<uint32_t const> indices) {
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

    return NEW<GLIndexBuffer>(IBO);
}

void GLRenderEngine::BindVertexBuffer(GLuint vbo) {
    Base::BindVertexBuffer(vbo);

    RunGL([=]() { glBindBuffer(GL_ARRAY_BUFFER, vbo); }, "Bind VBO");
}

void GLRenderEngine::BindIndexBuffer(GLuint ibo) {
    Base::BindIndexBuffer(ibo);

    RunGL([=]() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); }, "Bind IBO");
}

UP<SomeGLRenderCommand> GLRenderEngine::BuildRenderCommand(SomeRenderCommandModel& proxyCommand) {
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
    // Far/near are flipped in OpenGL
    projectionMatrix.LoadOrthographic(0, size.x, 0, size.y, Vector3::forward.z, Vector3::back.z);
}

void GLRenderEngine::ProjectionMatrixLoadPerspective(
    float fovRadians, float aspect, float zNear, float zFar
) {
    projectionMatrix.LoadPerspective(fovRadians, aspect, zNear, zFar);
}

void GLRenderEngine::LoadTranslate(Vector3 value) {
    viewMatrix.LoadTranslate(value);
}

void GLRenderEngine::RenderStart(SomeRenderContext* context) {
    renderPlans.clear();
    RecycleBufferPools();
}

void GLRenderEngine::RecycleBufferPools() {
    size_t const vertexInUse = inUseVertexBuffers.size();
    size_t const indexInUse = inUseIndexBuffers.size();

    for (auto& buffer : inUseVertexBuffers) {
        freeVertexBuffers.push_back(std::move(buffer));
    }
    inUseVertexBuffers.clear();

    for (auto& buffer : inUseIndexBuffers) {
        freeIndexBuffers.push_back(std::move(buffer));
    }
    inUseIndexBuffers.clear();

    // Trim free pools down to the recent peak in-use so spike frames don't permanently inflate
    // VRAM. Excess UPs destruct here, which calls glDeleteBuffers via GLSomeBuffer::~.
    vertexBufferUseHistory[poolHistoryIndex] = vertexInUse;
    indexBufferUseHistory[poolHistoryIndex] = indexInUse;
    poolHistoryIndex = (poolHistoryIndex + 1) % poolHistoryFrames;

    size_t const vertexPeak = std::max(
        poolMinFloor,
        *std::max_element(vertexBufferUseHistory.begin(), vertexBufferUseHistory.end())
    );
    size_t const indexPeak = std::max(
        poolMinFloor, *std::max_element(indexBufferUseHistory.begin(), indexBufferUseHistory.end())
    );

    if (freeVertexBuffers.size() > vertexPeak) {
        freeVertexBuffers.resize(vertexPeak);
    }
    if (freeIndexBuffers.size() > indexPeak) {
        freeIndexBuffers.resize(indexPeak);
    }
}

void GLRenderEngine::UploadVertexBuffer(
    GLVertexBuffer& buffer, GLVertexBufferPlan const& plan, GLenum usage
) {
    GLsizei totalSize = 0;
    for (auto& item : plan.items) {
        totalSize += item->Size();
    }
    GUARD(totalSize > 0)

    RunGL([&]() { glBindBuffer(GL_ARRAY_BUFFER, buffer.glId); }, "Bind VBO");
    // Orphan + refill: glBufferData(NULL) lets the driver allocate fresh storage
    // so this upload doesn't synchronize with in-flight draws using the prior contents.
    RunGL([&]() { glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, usage); }, "VBO Data");

    buffer.attributes.clear();
    int offset = 0;
    for (auto& item : plan.items) {
        auto itemSize = item->Size();
        RunGL(
            [&]() { glBufferSubData(GL_ARRAY_BUFFER, offset, itemSize, item->dataPtr); },
            "VBO SubData"
        );

        buffer.attributes[item->attributeId].offset = offset;
        buffer.attributes[item->attributeId].glType = item->glType;
        buffer.attributes[item->attributeId].normalize = item->normalize;

        offset += itemSize;
    }
}

void GLRenderEngine::UploadIndexBuffer(
    GLIndexBuffer& buffer, std::span<uint32_t const> indices, GLenum usage
) {
    RunGL([&]() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.glId); }, "Bind IBO");
    RunGL(
        [&]() {
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), usage
            );
        },
        "IBO Data"
    );
}

GLVertexBuffer* GLRenderEngine::AcquirePooledVertexBuffer(GLVertexBufferPlan const& plan) {
    GLsizei totalSize = 0;
    for (auto& item : plan.items) {
        totalSize += item->Size();
    }
    GUARDR(totalSize > 0, nullptr)

    UP<GLVertexBuffer> buffer;
    if (!freeVertexBuffers.empty()) {
        buffer = std::move(freeVertexBuffers.back());
        freeVertexBuffers.pop_back();
    } else {
        GLuint glId{};
        RunGL([&]() { glGenBuffers(1, &glId); }, "Gen VBO (pool)");
        buffer = NEW<GLVertexBuffer>(glId);
    }

    UploadVertexBuffer(*buffer, plan, GL_DYNAMIC_DRAW);

    auto raw = buffer.get();
    inUseVertexBuffers.push_back(std::move(buffer));
    return raw;
}

GLIndexBuffer* GLRenderEngine::AcquirePooledIndexBuffer(std::span<uint32_t const> indices) {
    UP<GLIndexBuffer> buffer;
    if (!freeIndexBuffers.empty()) {
        buffer = std::move(freeIndexBuffers.back());
        freeIndexBuffers.pop_back();
    } else {
        GLuint glId{};
        RunGL([&]() { glGenBuffers(1, &glId); }, "Gen IBO (pool)");
        buffer = NEW<GLIndexBuffer>(glId);
    }

    UploadIndexBuffer(*buffer, indices, GL_DYNAMIC_DRAW);

    auto raw = buffer.get();
    inUseIndexBuffers.push_back(std::move(buffer));
    return raw;
}

namespace {
    /// Shared DFS walker for RenderDraw variants. Dispatches cores by type and recurses
    /// into children. Caller is responsible for the final flush after returning.
    void WalkRenderNode(
        GLRenderEngine& engine, RenderPassModel::NodeSharedPtr const& node,
        std::function<void()> const& flush
    ) {
        GUARD(node)

        auto const& core = node->core;
        if (core) {
            if (auto* push = dynamic_cast<StencilPushRenderModel const*>(core.get())) {
                flush();
                engine.RenderStencilPush(*push);
            } else if (dynamic_cast<StencilPopRenderModel const*>(core.get())) {
                flush();
                engine.RenderStencilPop();
            } else if (auto* materialModel = dynamic_cast<MaterialRenderModel const*>(core.get())) {
                engine.RenderProcess(*materialModel);
            }
        }
        for (auto& edge : node->Edges()) {
            GUARD_CONTINUE(edge && edge->toNode)

            auto child = edge->toNode->Value();
            GUARD_CONTINUE(child)

            WalkRenderNode(engine, SCAST<RenderPassModel::Node>(child), flush);
        }
    }
} // namespace

void GLRenderEngine::RenderDraw(RenderPassModel const& pass) {
    GUARD(pass.rootNode)

    // FUTURE: we're using DAG painter's order for now. In the future, enable depth test for
    // performance
    EnableFeature(RenderFeature::DepthTest, false);

    auto flush = [this]() {
        if (renderPlans.empty()) {
            return;
        }
        VectorList<GLRenderPlan*> orderedPlans;
        orderedPlans.reserve(renderPlans.size());
        for (auto& plan : renderPlans) {
            orderedPlans.push_back(plan.get());
        }
        RenderDrawPlans(orderedPlans);
        renderPlans.clear();
    };

    WalkRenderNode(*this, pass.rootNode, flush);
    flush();
}

void GLRenderEngine::RenderDrawSubtree(SomeRenderModelNode const& startNode) {
    // FUTURE: we're using DAG painter's order for now. In the future, enable depth test for
    // performance
    EnableFeature(RenderFeature::DepthTest, false);

    auto flush = [this]() {
        if (renderPlans.empty()) {
            return;
        }
        VectorList<GLRenderPlan*> orderedPlans;
        orderedPlans.reserve(renderPlans.size());
        for (auto& plan : renderPlans) {
            orderedPlans.push_back(plan.get());
        }
        RenderDrawPlans(orderedPlans);
        renderPlans.clear();
    };

    for (auto& edge : startNode.Edges()) {
        GUARD_CONTINUE(edge && edge->toNode)

        auto child = edge->toNode->Value();
        GUARD_CONTINUE(child)

        WalkRenderNode(*this, SCAST<RenderPassModel::Node>(child), flush);
    }
    flush();
}

void GLRenderEngine::RenderStencilPush(StencilPushRenderModel const& model) {
    EnableFeature(RenderFeature::StencilTest, true);

    if (stencilDepth == 0) {
        // Fresh outer scope — the prior frame may have left the stencil write-mask at
        // 0x00 (set while content draws were active to protect the region). A 0 mask also
        // blocks glClear(GL_STENCIL_BUFFER_BIT), so force it open before clearing.
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, stencilDepth + 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // Draw the mesh into the stencil buffer
    if (model.material) {
        auto savedPlans = std::move(renderPlans);
        renderPlans.clear();

        MaterialRenderModel stencilDrawModel(model.material);
        stencilDrawModel.SetMesh(model.mesh);
        stencilDrawModel.matrix = model.matrix;

        // The shared colorVary shader expects one vertex color per position. Color writes
        // are masked off for this draw, so the actual value is irrelevant
        VectorList<RenderColor> stencilVertexColors(
            model.mesh.Vertices().size(), RenderColor(255, 255, 255, 255)
        );
        stencilDrawModel.SetVertexColors(stencilVertexColors);

        RenderProcess(stencilDrawModel);

        VectorList<GLRenderPlan*> immediatePlans;
        immediatePlans.reserve(renderPlans.size());
        for (auto& plan : renderPlans) {
            immediatePlans.push_back(plan.get());
        }
        RenderDrawPlans(immediatePlans);

        renderPlans = std::move(savedPlans);
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    ++stencilDepth;

    // Subsequent draws are clipped to where stencil >= depth.
    glStencilMask(0x00);
    glStencilFunc(GL_LEQUAL, stencilDepth, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void GLRenderEngine::RenderStencilPop() {
    if (stencilDepth == 0) {
        // Mismatched pop; still restore the write-mask so a later context Clear can wipe
        // the stencil buffer (glClear honors the current stencil write mask).
        glStencilMask(0xFF);
        EnableFeature(RenderFeature::StencilTest, false);
        return;
    }

    --stencilDepth;
    if (stencilDepth == 0) {
        // End of the outermost scope — open the mask so subsequent glClear calls can
        // clear the stencil. Without this, switching scenes leaves the prior
        // scene's stencil shape lingering because the mask is still 0x00 from the last
        // push's content-protect phase.
        glStencilMask(0xFF);
        EnableFeature(RenderFeature::StencilTest, false);
        return;
    }

    // Restore the test for the now-outer stencil scope.
    glStencilMask(0x00);
    glStencilFunc(GL_LEQUAL, stencilDepth, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void GLRenderEngine::RenderProcess(MaterialRenderModel const& model) {
    auto modelMaterial = model.Material();

    GUARD(modelMaterial)
    auto glProgram = As<GLShaderProgram>(modelMaterial->ShaderProgram());
    if (nullptr == glProgram) {
        PJ::Log("ERROR. GLShaderProgram required");
        return;
    }

    auto& mesh = model.GetMesh();
    auto& vertices = mesh.Vertices();
    auto& uvs = mesh.UVs();

    auto _vboPlan = NEW<GLVertexBufferPlan>();
    auto& vboPlan = *_vboPlan.get();

    std::span<Vector3 const> verticesSpan(vertices);
    vboPlan.Add("a_position", verticesSpan, GL_FLOAT);

    auto hasColorAttribute = glProgram->HasVertexAttribute("a_color");
    if (hasColorAttribute) {
        auto vertexColors = model.VertexColors();

        if (vertexColors.size() != vertices.size()) {
            PJ::Log("ERROR. Colors size does not match vertices size");
            return;
        }

        vboPlan.Add("a_color", model.VertexColors(), GL_UNSIGNED_BYTE, true);
    }

    // TODO: we shouldn't be copying texture coordinates if we're not using them
    auto hasTextureCoordAttribute = glProgram->HasVertexAttribute("a_texCoord");
    if (hasTextureCoordAttribute) {
        if (uvs.size() != vertices.size()) {
            PJ::Log("ERROR. UVs size does not match vertices size");
            return;
        }

        // AddStorage (copy data) is faster than Add (no copy). Not sure why
        std::span<Vector2 const> texCoordsSpan(uvs);
        vboPlan.AddStorage("a_texCoord", texCoordsSpan, GL_FLOAT);
    }

    auto renderPlan = NEW<GLRenderPlan>(model, _vboPlan);
    renderPlans.push_back(std::move(renderPlan));
}

void GLRenderEngine::RenderDrawPlans(std::span<GLRenderPlan* const> renderPlans) {
    Matrix4x4 const viewProjectionMatrix = projectionMatrix * viewMatrix;

    for (auto& rp : renderPlans) {
        auto& vboPlan = *rp->vboPlan.get();
        auto& model = rp->model;
        auto modelMaterial = model.Material();
        GUARD_CONTINUE(modelMaterial)

        auto glProgram = As<GLShaderProgram>(modelMaterial->ShaderProgram());
        GUARD(glProgram);

        auto& modelMatrix = model.matrix;

        auto vbo = AcquirePooledVertexBuffer(vboPlan);
        GUARD_CONTINUE(vbo)

        auto& mesh = model.GetMesh();
        auto ibo = AcquirePooledIndexBuffer(mesh.Triangles());
        GUARD_CONTINUE(ibo)

        auto hasColorAttribute = glProgram->HasVertexAttribute("a_color");
        auto hasTextureCoordAttribute = glProgram->HasVertexAttribute("a_texCoord");

        // MARK: - Uniforms

        Use(*glProgram);

        auto& textures = modelMaterial->Textures();
        for (size_t i = 0; i < textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + (GLenum)i);
            glBindTexture(GL_TEXTURE_2D, textures[i]->RenderId());

            // Shader sampler convention: u_texture is unit 0, u_texture1.. for additional units
            String samplerName = (0 == i) ? String("u_texture") : "u_texture" + std::to_string(i);
            if (glProgram->HasUniform(samplerName)) {
                glUniform1i(glProgram->uniformLocations[samplerName], (GLint)i);
            }
        }
        if (!textures.empty()) {
            glActiveTexture(GL_TEXTURE0);
        }

        if (glProgram->uniformLocations.find("u_mvpMatrix") != glProgram->uniformLocations.end()) {
            Matrix4x4 modelViewProjection = viewProjectionMatrix * modelMatrix;

            auto location = glProgram->uniformLocations["u_mvpMatrix"];
            UniformMatrix4fv(location, modelViewProjection.data());
        }

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

        DrawElements(GL_TRIANGLES, (GLsizei)mesh.Triangles().size(), GL_UNSIGNED_INT, nullptr);
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

    PJ::Log("****** OPENGL EXTENSIONS ******");
    for (auto& extension : glExtensions) {
        PJ::Log(extension);
    }
    PJ::Log("******                   ******");
}

SP<SomeRenderContext> GLRenderEngine::MakeTextureBuffer() {
    return MAKE<GLTextureBuffer>(*this);
}

VectorList<Tags> GLRenderEngine::EditorInfoList() {
    auto result = Base::EditorInfoList();

    auto addTag = [&](String label, size_t value) {
        Tags tags;
        tags.Set("label", label);
        tags.Set("value", String(std::to_string(value)));
        result.push_back(tags);
    };

    addTag("free vertex buffers size", freeVertexBuffers.size());
    addTag("free index buffers size", freeIndexBuffers.size());
    addTag("in use vertex buffers size", inUseVertexBuffers.size());
    addTag("in use index buffers size", inUseIndexBuffers.size());

    return result;
}
