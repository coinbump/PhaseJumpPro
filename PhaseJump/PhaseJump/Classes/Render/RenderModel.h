#pragma once

#include "Color.h"
#include "List.h"
#include "Matrix4x4.h"
#include "Mesh.h"
#include "RenderMaterial.h"
#include "RenderTypes.h"
#include "SharedVector.h"
#include "SomeTexture.h"
#include "UnorderedMap.h"
#include "Vector3.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/17/23
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
        RenderMaterial* material = nullptr;

        /// Override material used to render mesh. Some render processors may alter the material
        RenderMaterial* overrideMaterial = nullptr;

        /// Varying values that are interpolated per-vertex
        SharedVector<RenderColor> vertexColors;

    public:
        using This = RenderModel;

        void SetMaterial(RenderMaterial* material) {
            this->material = material;
        }

        void SetOverrideMaterial(RenderMaterial* material) {
            this->overrideMaterial = material;
        }

        /// @return Returns the original material, even if there is an override
        RenderMaterial* BaseMaterial() const {
            return material;
        }

        /// @return Returns the override material if it exists, or the original material
        RenderMaterial* Material() const {
            return overrideMaterial ? overrideMaterial : material;
        }

        /// Unique id of object that generated this model
        /// Used for render order, and z order sorting
        String id;

        /// User-readable name for debugging
        String name;

        /// Mesh to be rendered
        Mesh mesh;

        /// (Optional) Relative render order
        /// Painted on to the model by a render processor
        uint32_t order = 0;

        /// (Optional). Z layer for render, allows object renders to be grouped in layers
        uint32_t zIndex = 0;

        /// (Optional). Allows model to pass render arguments to render processors
        Tags tags;

        /// (Optional). Allows model to pass render hints to render processors
        TypeTagSet typeTags;

        /// Absolute Z position in world space, derived from render order
        /// Painted on to the model by a render processor for Z-ordering in ortho camera space
        float z = 0;

        /// Matrix for transform from model-local to world space
        Matrix4x4 matrix;

        /// Allows us to pre-emptively resize vectors with default elements
        RenderModel() {}

        RenderModel(RenderMaterial* material) :
            material(material) {}

        VectorList<Vector3> const& Vertices() const {
            return mesh.Vertices();
        }

        /// When used, you take responsibility for recaculating the bounds and normals later
        VectorList<Vector3>& ModifiableVertices() {
            return mesh.ModifiableVertices();
        }

        void SetVertices(VectorList<Vector3> const& value) {
            mesh.SetVertices(value);
        }

        /// Used by IBO to determine vertex offsets
        std::span<uint32_t const> Indices() const {
            return mesh.Triangles();
        }

        VectorList<Vector2> const& UVs() const {
            return mesh.UVs();
        };

        VectorList<RenderColor>& ModifiableVertexColors() {
            return vertexColors.Modifiable();
        }

        std::span<RenderColor const> VertexColors() const {
            return vertexColors.Value();
        }

        void SetVertexColors(std::span<RenderColor const> value) {
            vertexColors = { value };
        }

        void SetModifiableVertexColors(VectorList<RenderColor> const& value) {
            vertexColors = { value };
        }

        /// @return Returns true if a render feature is enabled for this operation
        bool IsFeatureEnabled(String feature) const {
            GUARDR(material, false)

            // FUTURE: support feature defaults if needed
            auto i = material->Features().find(feature);
            if (i == material->Features().end()) {
                return false;
            }
            return i->second == RenderFeatureState::Enable;
        }

        This operator+(This const& rhs) const {
            This result = *this;
            result += rhs;
            return result;
        }

        This& operator+=(This const& rhs) {
            auto& result = *this;

            result.mesh += rhs.mesh;
            VectorList<RenderColor> rhsColors;
            rhsColors.assign(rhs.vertexColors.Value().begin(), rhs.vertexColors.Value().end());
            AddRange(result.ModifiableVertexColors(), rhsColors);

            return result;
        }
    };
} // namespace PJ
