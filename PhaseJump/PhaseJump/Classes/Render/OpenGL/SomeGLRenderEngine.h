#pragma once

#include "Base.h"
#include "Color.h"
#include "Data.h"
#include "GLHeaders.h"
#include "GLRenderState.h"
#include "Matrix4x4.h"
#include "OrderedSet.h"
#include "RenderModel.h"
#include "SomeRenderEngine.h"
#include "VectorList.h"

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

    enum class RenderStep { PreRender };

    enum class ColorFormat {
        // Send colors as a 4-component float color
        Float,

        // Send colors as a 4-byte color
        Byte
    };

    /// Plan for building a GL vertex buffer
    struct GLVertexBufferPlan {
        struct Item {
            String attributeId;
            uint32_t componentCount;
            uint32_t componentSize;
            GLenum glType;
            bool normalize;
            SP<Data> data;

            uint32_t Size() const {
                return componentSize * componentCount;
            }

            Item(
                String attributeId, uint32_t componentCount, uint32_t componentSize, GLenum glType,
                void* data, bool normalize = false
            ) :
                attributeId(attributeId),
                componentCount(componentCount),
                componentSize(componentSize),
                glType(glType),
                normalize(normalize) {
                this->data = MAKE<Data>();
                this->data->CopyIn(data, componentCount * componentSize);
            }
        };

        VectorList<Item> items;

        template <class T>
        void
        Add(String attributeId, CollectionData<T> const& collection, GLenum glType,
            bool normalize = false) {
            PJ::Add(
                items, Item(
                           attributeId, (uint32_t)collection.ItemCount(), collection.ItemSize(),
                           glType, collection.Data(), normalize
                       )
            );
        }

        void Add(String attributeId, VectorList<Vector3> const& components) {
            Add(attributeId, ToCollectionData(components), GL_FLOAT);
        }

        void Add(String attributeId, VectorList<Color> const& components) {
            Add(attributeId, ToCollectionData(components), GL_FLOAT);
        }

        void Add(String attributeId, VectorList<RGBAColor> const& components) {
            Add(attributeId, ToCollectionData(components), GL_UNSIGNED_BYTE, true);
        }

        void Add(String attributeId, VectorList<Vector2> const& components) {
            Add(attributeId, ToCollectionData(components), GL_FLOAT);
        }
    };

    struct GLRenderPlan {
        RenderModel model;
        GLVertexBufferPlan vboPlan;

        GLRenderPlan(RenderModel model, GLVertexBufferPlan vboPlan) :
            model(model),
            vboPlan(vboPlan) {}
    };

    /**
     Abstracts OpenGL render commands to allow for a subclass to implement
     actual renders Example: OpenGL vs OpenGLES
     */
    // FUTURE: DeferredGLRenderEngine that supports batching
    class SomeGLRenderEngine : public SomeRenderEngine {
    protected:
        GLRenderState renderState;

    public:
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;

        VectorList<SP<GLRenderPlan>> renderPlans;

        ColorFormat colorFormat = ColorFormat::Float;

        SomeGLRenderEngine();

        virtual void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);
        virtual void BindFrameBuffer(GLuint fb);

        /// Load current matrix for rendering
        virtual void LoadMatrix() = 0;
        virtual void Use(GLShaderProgram& program) = 0;
        virtual SP<GLVertexBuffer> BuildVertexBuffer(GLVertexBufferPlan const& plan) = 0;
        virtual SP<GLIndexBuffer> BuildIndexBuffer(VectorList<uint32_t> indices) = 0;
        virtual void BindVertexBuffer(GLuint vbo);
        virtual void BindIndexBuffer(GLuint ibo);
        virtual void BindVertexArray(GLuint vao);

        void BindIndexBuffer(GLIndexBuffer ibo);
        void BindVertexBuffer(GLVertexBuffer vbo);

        virtual void EnableVertexAttributeArray(GLuint index, bool isEnabled);
        virtual void DisableAllVertexAttributeArrays();
        virtual void EnableOnlyVertexAttributeArrays(OrderedSet<GLuint> attributeLocations);

        virtual void UniformMatrix4fv(GLint location, const GLfloat* value);

        // IMPORTANT: the pointer parameter does not represent a pointer in
        // modern OpenGL Instead it is an offset into the Vertex Buffer
        virtual void VertexAttributePointer(
            GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
            const GLvoid* pointer
        ) = 0;
        virtual void DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) = 0;
        virtual void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) = 0;
        virtual void RunGL(std::function<void()> command, String name) = 0;
        virtual void SetBlendMode(GLBlendMode blendMode);

        // MARK: - SomeRenderEngine

        void EnableFeature(String featureId, bool isEnabled) override;
        void SetLineWidth(float lineWidth) override;

        void EnableOnlyFeatures(OrderedSet<String> features);

    protected:
        virtual void RunRender(std::function<void()> render);
    };
} // namespace PJ
