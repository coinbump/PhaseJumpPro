#pragma once

#include "Color.h"
#include "List.h"
#include "Matrix4x4.h"
#include "Mesh.h"
#include "RenderMaterial.h"
#include "RenderTypes.h"
#include "SharedVectorList.h"
#include "SomeTexture.h"
#include "UnorderedMap.h"
#include "Vector3.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    class SomeShaderProgram;
    class SomeRenderCommandModel;

    /**
     Model for a render operation

     Sent to the render engine which interprets the model, does any necessary batching or caching,
     and performs the render

     VectorList is used for easy n-element access and so we can directly upload to the render
     buffers. All render model types must be non-polymorphic to allow this
     */
    struct RenderModel {
    public:
        using This = RenderModel;

    protected:
        /// Material used to render mesh. Sharing materials between objects allows batching
        RenderMaterial* material{};

        /// Varying values that are interpolated per-vertex
        /// Optimize: Shared so that we can store the colors span from the renderer model without an
        /// extra copy
        SharedVectorList<RenderColor> vertexColors;

        /// Mesh to be rendered
        Mesh mesh;

        /// Mesh pointer to be rendered
        /// Optimize: use this to avoid unnecessary copies
        Mesh const* meshPtr{};

    public:
        /// @return Returns the material
        RenderMaterial* Material() const {
            return material;
        }

        void SetMaterial(RenderMaterial* value) {
            material = value;
        }

        /// Unique id of object that generated this model
        String id;

        /// User-readable name for debugging
        String name;

        /// Z layer for render, allows object renders to be grouped in layers
        /// (Doesn't work currently)
        uint32_t zIndex{};

        /// Custom properties and render hints
        Tags tags;

        /// Typed render hints
        TypeTagSet typeTags;

        /// Relative render order
        /// Painted on to the model by a render processor
        uint32_t order{};

        /// Absolute Z position in world space, derived from render order
        /// Painted on to the model by a render processor for Z-ordering in ortho camera space
        float z{};

        /// Matrix for transform from model to world space
        Matrix4x4 matrix;

        /// Allows us to pre-emptively resize vectors with default elements
        RenderModel() {}

        RenderModel(RenderMaterial* material) :
            material(material) {}

        Mesh& ModifiableMesh() {
            if (meshPtr) {
                mesh = *meshPtr;
                meshPtr = {};
            }
            return mesh;
        }

        Mesh const& GetMesh() const {
            return meshPtr ? *meshPtr : mesh;
        }

        void SetMesh(Mesh const& value) {
            mesh = value;
        }

        void SetMeshPtr(Mesh const* value) {
            meshPtr = value;
        }

        bool IsValid() const {
            return GetMesh().Vertices().size() > 0;
        }

        std::span<RenderColor const> VertexColors() const {
            return vertexColors.Span();
        }

        VectorList<RenderColor>& ModifiableVertexColors() {
            return vertexColors.Modifiable();
        }

        void SetVertexColors(std::span<RenderColor const> value) {
            vertexColors = value;
        }

        void SetVertexColors(VectorList<RenderColor> const& value) {
            vertexColors = value;
        }

        /// @return Returns true if a render feature is enabled for this operation
        bool IsFeatureEnabled(String feature) const;
    };
} // namespace PJ
