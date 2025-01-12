#pragma once

#include "SomeGLRenderEngine.h"
#include "UnorderedMap.h"
#include <optional>
#include <type_traits>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 1/12/25
 */
namespace PJ {
    class SomeGLRenderCommand;
    class SomeRenderCommandModel;

    // These types are tightly packed in VRAM for renders and cannot be polymorphic
    static_assert(!std::is_polymorphic_v<Vector2>);
    static_assert(!std::is_polymorphic_v<Vector3>);
    static_assert(!std::is_polymorphic_v<Color>);
    static_assert(!std::is_polymorphic_v<RGBAColor>);

    // FUTURE: add state-change optimizations by checking render state
    class GLRenderEngine : public SomeGLRenderEngine {
    protected:
        UnorderedMap<String, GLenum> featureIdToGLFeatureIdMap;

        UP<SomeGLRenderCommand> BuildRenderCommand(SomeRenderCommandModel& proxyCommand);

        virtual void RenderProcess(RenderModel const& model);

    public:
        using Base = SomeGLRenderEngine;
        using This = GLRenderEngine;

        GLuint vao{};

        /// @return Returns the OpenGL id for the given feature id
        std::optional<GLenum> FeatureIdToGLFeatureId(String featureId);

        // MARK: SomeGLRenderEngine

        void DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) override;
        void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) override;
        void EnableFeature(String featureId, bool isEnabled) override;
        void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height) override;
        void BindFrameBuffer(GLuint id) override;
        void BindTexture2D(GLuint id);
        void BindRenderBuffer(GLenum target, GLuint id);
        void UniformMatrix4fv(GLint location, const GLfloat* value) override;
        void EnableVertexAttributeArray(GLuint location, bool isEnabled) override;
        void VertexAttributePointer(
            GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
            const GLvoid* pointer
        ) override;
        void Use(GLShaderProgram& program) override;
        void RunGL(std::function<void()> command, String name) override;
        UP<GLVertexBuffer> BuildVertexBuffer(GLVertexBufferPlan const& plan) override;
        UP<GLIndexBuffer> BuildIndexBuffer(std::span<uint32_t const> indices) override;
        void BindVertexBuffer(GLuint vbo) override;
        void BindIndexBuffer(GLuint ibo) override;
        void BindVertexArray(GLuint vao) override;
        void SetBlendMode(GLBlendMode blendMode) override;

        // FUTURE: Implement if needed
        void LoadMatrix() override {}

        void RenderStart(SomeRenderContext* context) override;
        void RenderDraw(RenderDrawModel const& drawModel) override;
        void ProjectionMatrixLoadOrthographic(Vector2 size) override;
        void LoadTranslate(Vector3 value) override;
        SP<SomeRenderContext> MakeTextureBuffer() override;

    protected:
        void RenderProcess(RenderDrawModel const& processModel);
        void RenderDrawPlans(VectorList<GLRenderPlan*> const& renderPlans);
        void ScanGLExtensions();

        // MARK: Base

        void OnGo() override;
    };
} // namespace PJ
