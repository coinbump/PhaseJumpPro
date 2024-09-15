#pragma once

#include "Mesh.h"
#include "RenderTypes.h"
#include "RGBAColor.h"
#include "SomeRenderer.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    /// Renders a uniform color
    class ColorRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;
        using This = ColorRenderer;

        ColorRenderer(Color color, Vector2 worldSize);
        ColorRenderer(SP<RenderMaterial> material, Color color, Vector2 worldSize);

        static SP<RenderMaterial> MakeMaterial(RenderOpacityType opacityType);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ColorRenderer";
        }
    };
} // namespace PJ
