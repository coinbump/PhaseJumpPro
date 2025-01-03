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
    protected:
        /// Material used to render mesh. Sharing materials between objects allows batching
        RenderMaterial* material{};

        /// Varying values that are interpolated per-vertex
        /// Optimize: Shared so that we can store the colors span from the renderer model without an
        /// extra copy
        SharedVectorList<RenderColor> vertexColors;

    public:
        using This = RenderModel;

        /// @return Returns the material
        RenderMaterial* Material() const {
            return material;
        }

        void SetMaterial(RenderMaterial* material) {
            this->material = material;
        }

        /// Unique id of object that generated this model
        /// Used for render order, and z order sorting
        String id;

        /// User-readable name for debugging
        String name;

        /// Mesh to be rendered
        Mesh mesh;

        /// Z layer for render, allows object renders to be grouped in layers
        /// NOTE: currently zIndex is ignored
        uint32_t zIndex{};

        /// Custom properties and render hints for render system
        Tags tags;

        /// Allows model to pass render hints to render processors
        TypeTagSet typeTags;

        /// Relative render order
        /// Painted on to the model by a render processor
        uint32_t order{};

        /// Absolute Z position in world space, derived from render order
        /// Painted on to the model by a render processor for Z-ordering in ortho camera space
        float z{};

        /// Matrix for transform from model-local to world space
        Matrix4x4 matrix;

        /// Allows us to pre-emptively resize vectors with default elements
        RenderModel() {}

        RenderModel(RenderMaterial* material) :
            material(material) {}

        bool IsValid() const {
            return mesh.Vertices().size() > 0;
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
