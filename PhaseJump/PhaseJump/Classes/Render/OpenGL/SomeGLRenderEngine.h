#ifndef PJSOMEGLRENDERENGINE_H
#define PJSOMEGLRENDERENGINE_H

#include "Color.h"
#include "VectorList.h"
#include "GLHeaders.h"
#include "GLRenderState.h"
#include "SomeRenderEngine.h"
#include "Base.h"
#include "_Set.h"
#include "Matrix4x4.h"

/*
 RATING: 4 stars
 Tested and works
 Missing some functionality (Render line, lineLoop, etc.)
 */
namespace PJ {
    class GLVertexBuffer;
    class GLIndexBuffer;
    class GLShaderProgram;
    class Vector3;

    enum class RenderStep {
        PreRender
    };

    // Information for a render operation in OpenGL
    struct GLRenderModel {
        GLuint vbo = 0;
        GLuint ibo = 0;
        GLuint iboOffset = 0;
    };

    /**
     Abstracts OpenGL render commands to allow for a subclass to implement actual renders
     Example: OpenGL vs OpenGLES
     */
    // FUTURE: DeferredGLRenderEngine that supports batching
    class SomeGLRenderEngine : public SomeRenderEngine
    {
    protected:
        GLRenderState renderState;

        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;

    public:
        SomeGLRenderEngine();

        virtual void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);
        virtual void BindFrameBuffer(GLuint fb);

        /// Load current matrix for rendering
        virtual void LoadMatrix() = 0;
        virtual void Use(GLShaderProgram& program) = 0;
        virtual std::shared_ptr<GLVertexBuffer> BuildVertexBuffer(VectorList<Vector3> const& vertices) = 0;
        virtual std::shared_ptr<GLIndexBuffer> BuildIndexBuffer(VectorList<uint32_t> indices) = 0;
        virtual void BindVertexBuffer(GLuint vbo);
        virtual void BindIndexBuffer(GLuint ibo);
        virtual void BindVertexArray(GLuint vao);

        void BindIndexBuffer(GLIndexBuffer ibo);
        void BindVertexBuffer(GLVertexBuffer vbo);

        virtual void EnableVertexAttributeArray(GLuint index, bool isEnabled);
        virtual void DisableAllVertexAttributeArrays();
        virtual void EnableOnlyVertexAttributeArrays(Set<GLuint> attributeLocations);

        virtual void UniformMatrix4fv(GLint location, const GLfloat* value);

        // IMPORTANT: the pointer parameter does not represent a pointer in modern OpenGL
        // Instead it is an offset into the Vertex Buffer
        virtual void VertexAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) = 0;
        virtual void DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) = 0;
        virtual void DrawElements(GLenum mode, GLsizei count, GLenum type, const void * indices) = 0;
        virtual void RunGL(std::function<void()> command, String name) = 0;

        // MARK: - SomeRenderEngine

        void EnableFeature(String featureId, bool isEnabled) override;
        void SetLineWidth(float lineWidth) override;

        // FUTURE: implement as needed
        void RenderLineLoop(VectorList<float> const& vertices) override {}
        void RenderLine(VectorList<float> const& vertices) override {}

    protected:
        virtual void RunRender(std::function<void()> render);
        virtual void PreRender() {}    // Called before any render operation
    };
}

#endif
