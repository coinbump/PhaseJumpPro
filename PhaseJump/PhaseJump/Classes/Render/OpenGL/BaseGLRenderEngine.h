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
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 1/12/25
 */
namespace PJ {
    class GLVertexBuffer;
    class GLIndexBuffer;
    class GLShaderProgram;
    class Vector3;

    enum class ColorFormat {
        // Send colors as a 4-component float color
        Float,

        // Send colors as a 4-byte color
        Byte
    };

    /// Plan for building a GL vertex buffer
    struct GLVertexBufferPlan {
        /// A vertex buffer consists of item groups like vertices, colors, etc.
        /// This defines the data, count, and size for each VBO item group
        class Item {
        public:
            String attributeId;
            uint32_t componentCount{};
            uint32_t componentSize{};
            GLenum glType{};
            bool normalize{};
            void* dataPtr{};

            uint32_t Size() const {
                return componentSize * componentCount;
            }
        };

        /// Stores the typed-span for a VBO item
        template <class Type>
        struct DataItem : public Item {
            std::span<Type const> data;

            DataItem(
                String attributeId, uint32_t componentCount, uint32_t componentSize, GLenum glType,
                std::span<Type const> data, bool normalize = false
            ) :
                data(data) {
                this->attributeId = attributeId;
                this->componentCount = componentCount;
                this->componentSize = componentSize;
                this->glType = glType;
                this->normalize = normalize;
                this->dataPtr = (void*)(data.data());
            }
        };

        /// Stores the typed-span for a VBO item with a copy of the data
        template <class Type>
        struct StorageItem : public Item {
            Data<Type> data;

            StorageItem(
                String attributeId, uint32_t componentCount, uint32_t componentSize, GLenum glType,
                std::span<Type const> _data, bool normalize = false
            ) :
                data(_data) {
                this->attributeId = attributeId;
                this->componentCount = componentCount;
                this->componentSize = componentSize;
                this->glType = glType;
                this->normalize = normalize;
                this->dataPtr = (void*)(data.Pointer());
            }
        };

        /// Items that tell the render engine how to build the VBO
        VectorList<UP<Item>> items;

        /// Adds a new plan item
        template <class Type>
        void
        Add(String attributeId, std::span<Type const> collection, GLenum glType,
            bool normalize = false) {
            items.push_back(NEW<DataItem<Type>>(
                attributeId, (uint32_t)collection.size(),
                (uint32_t)(collection.size_bytes() / collection.size()), glType, collection,
                normalize
            ));
        }

        /// Adds a new plan storage item
        template <class Type>
        void AddStorage(
            String attributeId, std::span<Type const> collection, GLenum glType,
            bool normalize = false
        ) {
            items.push_back(NEW<StorageItem<Type>>(
                attributeId, (uint32_t)collection.size(),
                (uint32_t)(collection.size_bytes() / collection.size()), glType, collection,
                normalize
            ));
        }
    };

    /// Stores a render model, and a plan for building the VBO for that render model
    struct GLRenderPlan {
        RenderModel const& model;
        UP<GLVertexBufferPlan> vboPlan;

        GLRenderPlan(RenderModel const& model, UP<GLVertexBufferPlan>& vboPlan) :
            model(model),
            vboPlan(std::move(vboPlan)) {}
    };

    /**
     Abstracts OpenGL render commands to allow for a subclass to implement
     actual renders.

     Needed because OpenGL and OpenGL ES diverge
     */
    class BaseGLRenderEngine : public SomeRenderEngine {
    protected:
        // FUTURE: Support OpengL ES if needed
        /// Stores the active render state (work in progress)
        GLRenderState renderState;

        /// Render plans for each render model
        VectorList<UP<GLRenderPlan>> renderPlans;

    public:
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;

        /// If true, render states won't be switched if we're already in that state
        /// Flag enables us to A/B benchmark with this on/off
        bool optimizeStateSwitches = true;

        /// Color format for sending colors to OpenGL
        ColorFormat colorFormat = ColorFormat::Byte;

        BaseGLRenderEngine();

        virtual void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);
        virtual void BindFrameBuffer(GLuint fb);

        /// Loads current matrix for rendering
        virtual void LoadMatrix() = 0;

        /// Binds a shader program
        virtual void Use(GLShaderProgram& program) = 0;

        /// Buids a vertex buffer from a plan
        virtual UP<GLVertexBuffer> BuildVertexBuffer(GLVertexBufferPlan const& plan) = 0;

        /// Builds an index buffer
        virtual UP<GLIndexBuffer> BuildIndexBuffer(std::span<uint32_t const> indices) = 0;

        /// Binds the specified vertex buffer
        virtual void BindVertexBuffer(GLuint vbo);

        /// Binds the specified index buffer
        virtual void BindIndexBuffer(GLuint ibo);

        /// Binds the specified VAO
        virtual void BindVertexArray(GLuint vao);

        /// Enables the specified vertex attribute array
        virtual void EnableVertexAttributeArray(GLuint index, bool isEnabled);

        /// Disables all vertex attribute arrays
        virtual void DisableAllVertexAttributeArrays();

        /// Enables only the specified set of vertex attribute arrays
        virtual void EnableOnlyVertexAttributeArrays(UnorderedSet<GLuint> attributeLocations);

        /// Specifies a uniform matrix value for the current shader
        virtual void UniformMatrix4fv(GLint location, GLfloat const* value);

        // IMPORTANT: the pointer parameter does not represent a pointer in
        // modern OpenGL. Instead it is an offset into the vertex buffer
        virtual void VertexAttributePointer(
            GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
            const GLvoid* pointer
        ) = 0;

        /// Draws arrays
        virtual void DrawArrays(GLenum drawMode, GLint drawFirst, GLsizei drawCount) = 0;

        /// Draws elements
        virtual void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) = 0;

        /// Runs an OpenGL command, with logging
        virtual void RunGL(std::function<void()> command, String name) = 0;

        /// Sets the blend mode
        virtual void SetBlendMode(GLBlendMode blendMode);

        // MARK: - SomeRenderEngine

        void EnableFeature(String featureId, bool isEnabled) override;

        UnorderedSet<String> EnabledFeatures() override {
            return renderState.enabledFeatures;
        }

        void RenderPassStart() override {
            renderState.RenderPassStart();
        }

        bool IsContextCleared(uint32_t id) override {
            return renderState.IsContextCleared(id);
        }

        void SetIsContextCleared(uint32_t id, bool value) override {
            renderState.SetIsContextCleared(id, value);
        }

        void EnableOnlyFeatures(OrderedSet<String> features);

    protected:
        virtual void RunRender(std::function<void()> render);
    };
} // namespace PJ
