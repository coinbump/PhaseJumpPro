#ifndef PJRENDERTYPES_H
#define PJRENDERTYPES_H

namespace PJ {
    enum class RenderFeatureStatus {
        Enable,

        Disable
    };

    enum class TextureAlphaMode {
        Standard,

        PremultiplyAlpha
    };

    enum class TextureMagnification {
        Nearest,
        Linear
    };

    enum class TextureWrap {
        Clamp,
        Repeat
    };
}

#endif
