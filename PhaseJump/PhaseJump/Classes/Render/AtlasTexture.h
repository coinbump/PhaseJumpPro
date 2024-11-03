#pragma once

#include "Macros.h"
#include "SomeTexture.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/31/24
 */
namespace PJ {
    class TextureAtlas;

    /// GPU texture that references only part of its parent texture (used for texture atlas, fonts)
    class AtlasTexture : public SomeTexture {
    public:
        using Base = SomeTexture;
        using This = AtlasTexture;

        enum class Orientation {
            /// Texture uses original orientation
            Default,

            /// Texture is rotated 90 degrees
            Rotated
        };

        TextureAtlas* parent{};

        /// Properties to configure this type
        struct Config {
            Base::Config base;

            TextureAtlas* parent{};

            // FUTURE: not yet supported
            Orientation orientation = Orientation::Default;
        };

        AtlasTexture(Config const& config);

        SomeTexture* ParentTexture() const {
            return RenderTexture();
        }

        // MARK: SomeTexture

        SomeTexture* RenderTexture() const override;
        uint32_t RenderId() const override;
    };
} // namespace PJ
