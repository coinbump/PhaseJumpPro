#pragma once

#include "RGBAColor.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    class Color;

#define PJ_RENDER_32

    // Allows us to save memory if needed for render buffer colors
#if defined(PJ_RENDER_32)
    using RenderColor = Color32;
#else
    using RenderColor = Color;
#endif

    enum class RenderFeatureState {
        Enable,

        Disable
    };

    enum class RenderOpacityType {
        Opaque,

        Blend
    };

    std::ostream& operator<<(std::ostream& os, RenderFeatureState const& value);

    using TextureAlphaModeType = String;

    namespace TextureAlphaMode {
        auto constexpr Standard = "standard";
        auto constexpr PremultiplyAlpha = "premultiply.alpha";
    }; // namespace TextureAlphaMode

    using TextureMagnificationType = String;

    namespace TextureMagnification {
        auto constexpr Nearest = "nearest";
        auto constexpr Linear = "linear";
    }; // namespace TextureMagnification

    using TextureWrapType = String;

    namespace TextureWrap {
        auto constexpr Clamp = "clamp";
        auto constexpr Repeat = "repeat";
    }; // namespace TextureWrap
} // namespace PJ
