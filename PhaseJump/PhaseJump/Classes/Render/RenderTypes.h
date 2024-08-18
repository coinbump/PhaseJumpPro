#pragma once

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    enum class RenderFeatureStatus {
        Enable,

        Disable
    };

    std::ostream& operator<<(std::ostream& os, RenderFeatureStatus const& value);

    using TextureAlphaModeType = String;

    namespace TextureAlphaMode {
        static auto const Standard = "standard";
        static auto const PremultiplyAlpha = "premultiply.alpha";
    }; // namespace TextureAlphaMode

    using TextureMagnificationType = String;

    namespace TextureMagnification {
        static auto const Nearest = "nearest";
        static auto const Linear = "linear";
    }; // namespace TextureMagnification

    using TextureWrapType = String;

    namespace TextureWrap {
        static auto const Clamp = "clamp";
        static auto const Repeat = "repeat";
    }; // namespace TextureWrap
} // namespace PJ
