#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "UVRect.h"
#include <array>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/30/24
 */
namespace PJ {
    class RenderMaterial;
    struct Mesh;

    /// Renders a single texture as a 9-slice
    class Slice9TextureRenderer : public MaterialRenderer {
    public:
        using Base = MaterialRenderer;

        // TODO: stop using as a tag, use funcs instead
        /// Model that defines how to slice the texture
        struct SliceModel {
            /// Insets from top left that defines the top-left corner
            Vector2Int topLeftInsets;

            /// Insets from bottom right that defines the bottom-right corner
            Vector2Int bottomRightInsets;
        };

        struct Config {
            SP<Texture> texture;
            Vector2 worldSize;
            SliceModel sliceModel;
        };

    protected:
        /// Model defining where to slice
        SliceModel sliceModel;

        SP<Texture> texture;

    public:
        /// Model used to build the sliced mesh. Used for unit tests
        struct BuildModel {
            float sliceLeft{};
            float sliceTop{};
            float sliceRightOffset{};
            float sliceBottomOffset{};

            float sliceRight{};
            float sliceBottom{};

            float left{};
            float top{};
            float right{};
            float bottom{};

            float topAndBottomWorldWidth{};
            float leftAndRightWorldHeight{};
            float topAndBottomSliceWidth{};
            float leftAndRightSliceHeight{};

            Vector2Int textureSize;

            float uvLeft{};
            float uvLeftWidth{};
            float uvTopOffset{};
            float uvTopHeight{};
            float uvTop{};
            float uvBottom{};
            float uvBottomHeight{};
            float uvRight{};
            float uvRightWidth{};
            float uvTopAndBottomWidth{};
            float uvLeftAndRightHeight{};

            UVRect topLeftUVRect;
            UVRect topUVRect;
            UVRect topRightUVRect;
            UVRect leftUVRect;
            UVRect centerUVRect;
            UVRect rightUVRect;
            UVRect bottomLeftUVRect;
            UVRect bottomUVRect;
            UVRect bottomRightUVRect;
        };

        enum MeshIndex {
            TopLeft,
            Top,
            TopRight,
            Left,
            Center,
            Right,
            BottomLeft,
            Bottom,
            BottomRight
        };

        Slice9TextureRenderer(Config const& config);

        BuildModel MakeBuildModel(Vector3 worldSize) const;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Slice9TextureRenderer";
        }

    protected:
        Mesh BuildMesh(Vector3 worldSize);
    };
} // namespace PJ
