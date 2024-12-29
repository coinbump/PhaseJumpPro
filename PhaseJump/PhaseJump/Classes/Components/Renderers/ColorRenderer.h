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
    class ColorRenderer : public SomeMaterialRenderer {
    public:
#if PSEUDOCODE
        MIRROR_CLASS(ColorRendererClass);
#endif

        using Base = SomeMaterialRenderer;
        using This = ColorRenderer;

        struct Config {
            Color color;
            Vector2 worldSize;
            SP<RenderMaterial> material;
        };

        ColorRenderer(Config config);
        ColorRenderer(Color color, Vector2 worldSize);

        /// Disable dynamic blend mode changes
        void EnableBlend(bool isFeatureEnabled);

        static SP<RenderMaterial> MakeMaterial(RenderOpacityType opacityType);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ColorRenderer";
        }
    };
} // namespace PJ
