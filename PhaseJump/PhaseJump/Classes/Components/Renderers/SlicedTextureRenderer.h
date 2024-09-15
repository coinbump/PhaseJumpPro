#ifndef PJSLICEDTEXTURERENDERER_H
#define PJSLICEDTEXTURERENDERER_H

#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "UVRect.h"
#include <array>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/3/23
 */
namespace PJ {
    class RenderMaterial;
    struct Mesh;

    /// Renders a single texture as a 9-slice
    class SlicedTextureRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;

        struct SlicePoints {
            Vector2Int topLeft;
            Vector2Int bottomRight;

            SlicePoints() {}

            SlicePoints(Vector2Int topLeft, Vector2Int bottomRight) :
                topLeft(topLeft),
                bottomRight(bottomRight) {}
        };

    protected:
        /// First point is offset from top-left of texture
        SlicePoints slicePoints;

        /// Material holds the render texture, this is the actual texture object (child texture for
        /// texture atlas)
        SP<SomeTexture> texture;

    public:
        /// Model used to build the sliced mesh. Used for unit tests
        struct BuildModel {
            float sliceLeft = 0;
            float sliceTop = 0;
            float sliceRightOffset = 0;
            float sliceBottomOffset = 0;

            float sliceRight = 0;
            float sliceBottom = 0;

            float left = 0;
            float top = 0;
            float right = 0;
            float bottom = 0;

            float topAndBottomWorldWidth = 0;
            float leftAndRightWorldHeight = 0;
            float topAndBottomSliceWidth = 0;
            float leftAndRightSliceHeight = 0;

            Vector2Int textureSize;

            float uvLeft = 0;
            float uvLeftWidth = 0;
            float uvTopOffset = 0;
            float uvTopHeight = 0;
            float uvTop = 0;
            float uvBottom = 0;
            float uvBottomHeight = 0;
            float uvRight = 0;
            float uvRightWidth = 0;
            float uvTopAndBottomWidth = 0;
            float uvLeftAndRightHeight = 0;

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

        SlicedTextureRenderer(SP<SomeTexture> texture, Vector2 worldSize, SlicePoints slicePoints);

        BuildModel MakeBuildModel() const;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SlicedTextureRenderer";
        }

    protected:
        Mesh BuildMesh(Vector3 worldSize);
    };
} // namespace PJ

#endif
