#include "SomeGLRenderEngine.h"
#include "GLVertexBuffer.h"

using namespace PJ;

SomeGLRenderEngine::SomeGLRenderEngine() {
    viewMatrix.LoadIdentity();
    projectionMatrix.LoadIdentity();
}

void SomeGLRenderEngine::RunRender(std::function<void()> render) {
    PreRender();

    render();
}

void SomeGLRenderEngine::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    renderState.viewport = GLRenderState::Viewport(x, y, width, height);
}

void SomeGLRenderEngine::SetLineWidth(float lineWidth) {
    renderState.lineWidth = lineWidth;
}

void SomeGLRenderEngine::BindFrameBuffer(GLuint fb) {
    renderState.boundFrameBuffer = fb;
}

void SomeGLRenderEngine::EnableFeature(String featureId, bool isEnabled) {
    renderState.enabledFeatures.AddOrRemove(featureId, isEnabled);
}

void SomeGLRenderEngine::UniformMatrix4fv(GLint location, const GLfloat* value) {
}

void SomeGLRenderEngine::EnableVertexAttributeArray(GLuint index, bool isEnabled) {
    if (isEnabled) {
        renderState.activeAttributeLocations.Add(index);
    } else {
        renderState.activeAttributeLocations.Remove(index);
    }
}

void SomeGLRenderEngine::DisableAllVertexAttributeArrays() {
    auto iterLocations = renderState.activeAttributeLocations;
    for (auto location : iterLocations) {
        EnableVertexAttributeArray(location, false);
    }
}

void SomeGLRenderEngine::EnableOnlyVertexAttributeArrays(Set<GLuint> attributeLocations) {
    DisableAllVertexAttributeArrays();

    for (auto location : attributeLocations) {
        EnableVertexAttributeArray(location, true);
    }
}

void SomeGLRenderEngine::BindVertexBuffer(GLVertexBuffer vbo) {
    BindVertexBuffer(vbo.glId);
}

void SomeGLRenderEngine::BindIndexBuffer(GLIndexBuffer ibo) {
    BindIndexBuffer(ibo.glId);
}

void SomeGLRenderEngine::BindVertexBuffer(GLuint vbo)
{
    renderState.boundVBO = vbo;
}

void SomeGLRenderEngine::BindIndexBuffer(GLuint ibo)
{
    renderState.boundIBO = ibo;
}

void SomeGLRenderEngine::BindVertexArray(GLuint vao)
{
    renderState.boundVAO = vao;
}
