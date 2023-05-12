#ifndef PJSLICEDTEXTURERENDERER_H
#define PJSLICEDTEXTURERENDERER_H

#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "UVRect.h"
#include <memory>
#include <array>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/3/23
 */
namespace PJ {
    class RenderMaterial;
    class RenderMesh;

    /// Renders a single texture as a 9-slice
    class SlicedTextureRenderer: public SomeRenderer {
    protected:
        RenderMesh renderMesh;

        /// First point is offset from top-left of texture. Second point is offset from bottom-right
        std::array<Vector2Int, 2> slicePoints;
        Vector2 size;

    public:
        /// Model used to build the sliced mesh. Used for unit tests
        struct BuildModel {
            int sliceLeft = 0;
            int sliceTop =  0;
            int sliceRightOffset = 0;
            int sliceBottomOffset = 0;

            int sliceRight = 0;
            int sliceBottom = 0;

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
            TopLeft, Top, TopRight, Left, Center, Right, BottomLeft, Bottom, BottomRight
        };

        SP<RenderMaterial> material;

        SlicedTextureRenderer(SP<SomeTexture> texture, Vector2 worldSize, std::array<Vector2Int, 2> slicePoints);

        void SetSize(Vector2 worldSize);

        void RenderInto(RenderIntoModel model) override;
        BuildModel MakeBuildModel() const;

        std::optional<Vector3> WorldSize() const override { return std::make_optional(Vector3(size.x, size.y, 0)); }

    protected:
        void BuildRenderMesh();
    };
}

#endif
