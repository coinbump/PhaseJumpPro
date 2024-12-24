#pragma once

#include "Axis.h"
#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "UVRect.h"
#include <array>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/7/24
 */
namespace PJ {
    class RenderMaterial;
    struct Mesh;

    /// Renders a single texture as a 3-slice (start, middle, end)
    class Slice3TextureRenderer : public SomeMaterialRenderer {
    public:
        using Base = SomeMaterialRenderer;

    protected:
        SP<SomeTexture> texture;

        /// Which axis of the texture we're slicing
        Axis2D axis{};

        /// Inset for start cap
        float startInset{};

        /// Inset for end cap
        float endInset{};

    public:
        enum MeshIndex { Start, Center, End };

        /// Model used to build the sliced mesh. Used for unit tests
        struct BuildModel {
            struct Model {
                Vector2 worldSize;
                Vector2 tileSize;
                UVRect uvRect;

                bool IsValid() const {
                    return tileSize.x > 0 && tileSize.y > 0;
                }
            };

            Model start;
            Model center;
            Model end;

            bool IsValid() const {
                return start.IsValid() && center.IsValid() && end.IsValid();
            }
        };

        struct Config {
            SP<SomeTexture> texture;

            /// Which axis of the texture we're slicing
            Axis2D axis{};

            /// Length of rendered texture along the axis
            float axisLength = 100;

            /// Inset for start cap
            float startInset{};

            /// Inset for end cap
            float endInset{};
        };

        Slice3TextureRenderer(Config config);

        BuildModel MakeBuildModel(Vector3 worldSize) const;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Slice3TextureRenderer";
        }

    protected:
        Mesh BuildMesh(Vector3 worldSize);
    };
} // namespace PJ
