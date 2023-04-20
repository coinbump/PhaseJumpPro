#include "GLRenderEngine.h"
#include "GLHeaders.h"
#include "GLShaderProgram.h"
#include "GLVertexBuffer.h"
#include "GLShaderProgramLoader.h"
#include "Vector3.h"
#include "RenderFeatures.h"
#include "RenderModel.h"

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
    RunGL([&] () { glUseProgram(program.GLId()); }, "glUseProgram");

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

        GLShaderProgram::registry[info.id] = program;
    }

    featureIdToGLFeatureIdMap[RenderFeatures::Blend] = GL_BLEND;
    featureIdToGLFeatureIdMap[RenderFeatures::ScissorTest] = GL_SCISSOR_TEST;
}

void GLRenderEngine::BindVertexArray(GLuint vao) {
    Base::BindVertexArray(vao);

    RunGL([=] () { glBindVertexArray(vao); }, "glBindVertexArray");
}

std::shared_ptr<GLVertexBuffer> GLRenderEngine::BuildVertexBuffer(GLVertexBufferPlan const& plan)
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

    auto result = std::make_shared<GLVertexBuffer>(VBO);

    for (auto item : plan.items) {
        auto itemSize = item.Size();
        RunGL([&] () { glBufferSubData(GL_ARRAY_BUFFER, offset, itemSize, item.data); }, "VBO Data");

        result->attributeOffsets[item.attributeId] = offset;

        offset += itemSize;
    }

    return result;
}

std::shared_ptr<GLIndexBuffer> GLRenderEngine::BuildIndexBuffer(VectorList<uint32_t> indices) {
    GLuint IBO;
    RunGL([&] () { glGenBuffers(1, &IBO); }, "Gen IBO");
    RunGL([&] () { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); }, "Bind IBO");
    RunGL([&] () { glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW); }, "IBO Data");

    return std::make_shared<GLIndexBuffer>(IBO);
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

void GLRenderEngine::Render(RenderModel const& model)  {
    auto glProgram = dynamic_cast<GLShaderProgram*>(&model.shaderProgram);
    if (nullptr == glProgram) {
        PJLog("ERROR. GLShaderProgram required");
        return;
    }

    Use(*glProgram);

    projectionMatrix.LoadOrthographic(0, renderState.viewport.width, 0, renderState.viewport.height, -1, 1);
    viewMatrix.LoadTranslate(Vector3(renderState.viewport.width / 2.0f, renderState.viewport.height / 2.0f, 0));
    auto modelMatrix = model.matrix;

    VectorList<Vector3> vertices = model.vertices;
    auto vertexCount = model.vertices.size();
    VectorList<Color> colors(vertexCount);

    if (glProgram->uniformLocations.find("u_mvpMatrix") != glProgram->uniformLocations.end()) {
        Matrix4x4 viewProjectionMatrix = projectionMatrix * viewMatrix;
        Matrix4x4 modelViewProjection = viewProjectionMatrix * modelMatrix;

        auto location = glProgram->uniformLocations["u_mvpMatrix"];
        UniformMatrix4fv(location, modelViewProjection.data());
    }

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

            colors[i] = color;
        }
        vboPlan.Add("a_color", colors);
    }

    auto vbo = BuildVertexBuffer(vboPlan);
    auto ibo = BuildIndexBuffer(model.indices);

    if (model.uniformColors.size() > 0 && glProgram->HasUniform("u_color")) {
        auto color = model.uniformColors[0];
        glUniform4f(glProgram->uniformLocations["u_color"], color.r, color.g, color.b, color.a);
    }

    BindVertexBuffer(vbo->glId);
    BindIndexBuffer(ibo->glId);

    VertexAttributePointer(glProgram->attributeLocations["a_position"], 3, GL_FLOAT, false, 0, 0);

    if (hasColorAttribute) {
        VertexAttributePointer(glProgram->attributeLocations["a_color"], 4, GL_FLOAT, false, 0, (void*)(uintptr_t)vbo->attributeOffsets["a_color"]);
    }

    DrawElements(GL_TRIANGLES, (GLsizei)model.indices.size(), GL_UNSIGNED_INT, nullptr);
}