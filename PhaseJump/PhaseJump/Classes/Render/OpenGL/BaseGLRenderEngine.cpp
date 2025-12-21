#include "BaseGLRenderEngine.h"
#include "GLSomeBuffer.h"

using namespace PJ;

BaseGLRenderEngine::BaseGLRenderEngine() {
    viewMatrix.LoadIdentity();
    projectionMatrix.LoadIdentity();
}

void BaseGLRenderEngine::RunRender(std::function<void()> render) {
    GUARD(render)
    render();
}

void BaseGLRenderEngine::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    renderState.viewport = GLRenderState::Viewport(x, y, width, height);
}

void BaseGLRenderEngine::BindFrameBuffer(GLuint fb) {
    renderState.boundFrameBuffer = fb;
}

void BaseGLRenderEngine::SetBlendMode(GLBlendMode blendMode) {
    renderState.blendMode = blendMode;
}

void BaseGLRenderEngine::EnableFeature(String featureId, bool isEnabled) {
    AddOrRemove(renderState.enabledFeatures, featureId, isEnabled);
}

void BaseGLRenderEngine::EnableOnlyFeatures(OrderedSet<String> features) {
    OrderedSet<String> iterFeatures;

    std::set_difference(
        renderState.enabledFeatures.begin(), renderState.enabledFeatures.end(), features.begin(),
        features.end(), std::inserter(iterFeatures, iterFeatures.begin())
    );

    for (auto& feature : iterFeatures) {
        EnableFeature(feature, false);
    }

    for (auto& feature : features) {
        EnableFeature(feature, true);
    }
}

void BaseGLRenderEngine::UniformMatrix4fv(GLint location, const GLfloat* value) {}

void BaseGLRenderEngine::EnableVertexAttributeArray(GLuint index, bool isEnabled) {
    if (isEnabled) {
        renderState.activeAttributeLocations.insert(index);
    } else {
        renderState.activeAttributeLocations.erase(index);
    }
}

void BaseGLRenderEngine::DisableAllVertexAttributeArrays() {
    auto iterLocations = renderState.activeAttributeLocations;
    for (auto& location : iterLocations) {
        EnableVertexAttributeArray(location, false);
    }
}

void BaseGLRenderEngine::EnableOnlyVertexAttributeArrays(UnorderedSet<GLuint> attributeLocations) {
    if (optimizeStateSwitches && renderState.activeAttributeLocations == attributeLocations) {
        return;
    }

    DisableAllVertexAttributeArrays();

    for (auto& location : attributeLocations) {
        EnableVertexAttributeArray(location, true);
    }
}

void BaseGLRenderEngine::BindVertexBuffer(GLuint vbo) {
    renderState.boundVBO = vbo;
}

void BaseGLRenderEngine::BindIndexBuffer(GLuint ibo) {
    renderState.boundIBO = ibo;
}

void BaseGLRenderEngine::BindVertexArray(GLuint vao) {
    renderState.boundVAO = vao;
}
