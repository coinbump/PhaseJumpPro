#ifndef PJGLRENDERENGINE_H
#define PJGLRENDERENGINE_H

#include "SomeGLRenderEngine.h"
#include "UnorderedMap.h"
#include <optional>

/*
 RATING: 4 stars
 Tested and works
 Missing some features
 */
namespace PJ {
    class SomeGLRenderCommand;
    class SomeRenderCommandModel;

    // These types are tightly packed in VRAM for renders and cannot be polymorphic
    static_assert(!std::is_polymorphic_v<Vector2>);
    static_assert(!std::is_polymorphic_v<Vector3>);
    static_assert(!std::is_polymorphic_v<Color>);
    static_assert(!std::is_polymorphic_v<RGBAColor>);

    // FUTURE: add state-change optimizations by checking render state (will
    // imGui interfere?)
    class GLRenderEngine : public SomeGLRenderEngine {
    protected:
        UnorderedMap<String, GLenum> featureIdToGLFeatureIdMap;

        SP<SomeGLRenderCommand> BuildRenderCommand(SomeRenderCommandModel& proxyCommand);

        virtual void RenderProcess(RenderModel const& model);

    public:
        using Base = SomeGLRenderEngine;
        GLuint vao = 0;

        void DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) override;
        void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) override;

        void SetLineWidth(float lineWidth) override;
        void EnableFeature(String featureId, bool isEnabled) override;
        void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height) override;
        void BindFrameBuffer(GLuint fb) override;
        void UniformMatrix4fv(GLint location, const GLfloat* value) override;

        void EnableVertexAttributeArray(GLuint location, bool isEnabled) override;

        void VertexAttributePointer(
            GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
            const GLvoid* pointer
        ) override;
        void Use(GLShaderProgram& program) override;
        void RunGL(std::function<void()> command, String name) override;

        std::optional<GLenum> FeatureIdToGLFeatureId(String featureId);
        SP<GLVertexBuffer> BuildVertexBuffer(GLVertexBufferPlan const& plan) override;
        SP<GLIndexBuffer> BuildIndexBuffer(VectorList<uint32_t> indices) override;
        void BindVertexBuffer(GLuint vbo) override;
        void BindIndexBuffer(GLuint ibo) override;
        void BindVertexArray(GLuint vao) override;

        void SetBlendMode(GLBlendMode blendMode) override;

        // FUTURE: Implement as needed
        void LoadMatrix() override {}

        void RenderStart(RenderContextModel const& model) override;
        void RenderProcess(RenderProcessModel const& processModel) override;
        void RenderDraw() override;
        void RenderDrawPlans(VectorList<SP<GLRenderPlan>> const& renderPlans);

        void ProjectionMatrixLoadOrthographic(Vector2 size) override;
        void LoadTranslate(Vector3 value) override;

    protected:
        void GoInternal() override;
        void ScanGLExtensions();
    };
} // namespace PJ

#endif
