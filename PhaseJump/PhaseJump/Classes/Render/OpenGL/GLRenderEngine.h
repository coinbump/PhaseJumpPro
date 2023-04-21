#ifndef PJGLRENDERENGINE_H
#define PJGLRENDERENGINE_H

#include "SomeGLRenderEngine.h"
#include "_Map.h"
#include <optional>

/*
 RATING: 4 stars
 Tested and works
 Missing some features
 */
namespace PJ {
    // FUTURE: add state-change optimizations by checking render state (will imGui interfere?)
    class GLRenderEngine : public SomeGLRenderEngine {
    protected:
        Map<String, GLenum> featureIdToGLFeatureIdMap;

    public:
        using Base = SomeGLRenderEngine;
        GLuint vao;

        void DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) override;
        void DrawElements(GLenum mode, GLsizei count, GLenum type, const void * indices) override;

        void SetLineWidth(float lineWidth) override;
        void EnableFeature(String featureId, bool isEnabled) override;
        void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height) override;
        void BindFrameBuffer(GLuint fb) override;
        void UniformMatrix4fv(GLint location, const GLfloat* value) override;

        void EnableVertexAttributeArray(GLuint location, bool isEnabled) override;

        void VertexAttributePointer(GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) override;
        void Use(GLShaderProgram& program) override;
        void RunGL(std::function<void()> command, String name) override;

        std::optional<GLenum> FeatureIdToGLFeatureId(String featureId);
        std::shared_ptr<GLVertexBuffer> BuildVertexBuffer(GLVertexBufferPlan const& plan) override;
        std::shared_ptr<GLIndexBuffer> BuildIndexBuffer(VectorList<uint32_t> indices) override;
        void BindVertexBuffer(GLuint vbo) override;
        void BindIndexBuffer(GLuint ibo) override;
        void BindVertexArray(GLuint vao) override;

        void SetBlendMode(GLBlendMode blendMode) override;

        // FUTURE: Implement as needed
        void LoadMatrix() override {}
        void RenderStart() override;
        void RenderProcess(RenderModel const& model) override;
        void RenderDraw() override;

    protected:
        void GoInternal() override;
        void ScanGLExtensions();
    };
}

#endif
