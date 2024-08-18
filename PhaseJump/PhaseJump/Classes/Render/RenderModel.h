#pragma once

#include "Color.h"
#include "List.h"
#include "Matrix4x4.h"
#include "Mesh.h"
#include "RenderMaterial.h"
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
        using This = RenderModel;

        /// Mesh to be rendered
        Mesh mesh;

        /// Material used to render mesh. Sharing materials between objects allows batching
        RenderMaterial material;

        /// Render model for textures (used to specify texture coordinates for texture atlas)
        VectorList<TextureRenderModel> textureModels;

        // TODO: this is weird <- use comparator in render engine somewhere?
        /// Use to sort renders back-to-front (higher value is rendered after lower value)
        float z = 0;

        /// Matrix for transform from model-local to world space
        Matrix4x4 matrix;

        /// Varying values that are interpolated per-vertex
        VectorList<Color> colors;

        RenderModel(RenderMaterial const& material) :
            material(material) {}

        VectorList<Vector3>& Vertices() {
            return mesh.vertices;
        }

        VectorList<Vector3> const& Vertices() const {
            return mesh.vertices;
        }

        /// Used by IBO to determine vertex offsets
        VectorList<uint32_t>& Indices() {
            return mesh.triangles;
        }

        /// Used by IBO to determine vertex offsets
        VectorList<uint32_t> const& Indices() const {
            return mesh.triangles;
        }

        /// Texture position for vertex (Range: 0-1.0)
        VectorList<Vector2>& UVs() {
            return mesh.uvs;
        };

        VectorList<Vector2> const& UVs() const {
            return mesh.uvs;
        };

        VectorList<float> const& UniformFloats() const {
            return material.UniformFloats();
        }

        VectorList<Color> const& UniformColors() const {
            return material.UniformColors();
        }

        VectorList<Color>& Colors() {
            return colors;
        }

        VectorList<Color> const& Colors() const {
            return colors;
        }

        UnorderedMap<String, RenderFeatureStatus> const& Features() const {
            return material.Features();
        }

        SomeShaderProgram* ShaderProgram() const {
            return material.ShaderProgram().get();
        }

        /// Returns true if a render feature is enabled for this operation
        bool IsFeatureEnabled(String feature) const {
            // FUTURE: support feature defaults if needed
            auto i = material.Features().find(feature);
            if (i == material.Features().end()) {
                return false;
            }
            return i->second == RenderFeatureStatus::Enable;
        }

        This operator+(This const& rhs) const {
            This result = *this;
            result += rhs;
            return result;
        }

        This& operator+=(This const& rhs) {
            auto& result = *this;

            result.mesh += rhs.mesh;
            AddRange(result.colors, rhs.colors);

            return result;
        }
    };
} // namespace PJ
