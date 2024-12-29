#pragma once

#include "Base.h"
#include "Bitmap.h"
#include "RenderTypes.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    /// GPU texture that can be rendered in a Render Context
    class SomeTexture : public Base {
    protected:
        uint32_t renderId = 0;

    public:
        String id;
        Vector2Int size;

        String alphaMode = TextureAlphaMode::Standard;

        /// Texture origin in reading coordinates (top-left is 0, 0)
        Vector2Int origin;

        /// Normalized texture origin (0-1.0) in reading coordinates
        Vector2 normalOrigin;

        /// Normalized texture size (0-1.0)
        Vector2 normalSize;

        /// (Optional). Distance of trim from top-left of original image
        Vector2Int trimOrigin;

        /// (Optional). Original texture size before trim
        Vector2Int untrimmedSize;

        struct Config {
            String id;

            // Method A: prebuilt texture
            uint32_t renderId{};
            Vector2Int origin;
            Vector2Int size;
            Vector2Int trimOrigin;
            std::optional<Vector2Int> untrimmedSize;
            Vector2 normalOrigin;
            Vector2 normalSize = Vector2::one;
            String alphaMode = TextureAlphaMode::Standard;
            String textureMagnification = TextureMagnify::Linear;

            // Method: B: convert from bitmap
            RGBABitmap* bitmap{};
        };

        SomeTexture(Config const& config) :
            id(config.id),
            renderId(config.renderId),
            origin(config.origin),
            size(config.size),
            normalOrigin(config.normalOrigin),
            normalSize(config.normalSize),
            trimOrigin(config.trimOrigin),
            untrimmedSize(config.untrimmedSize ? *config.untrimmedSize : config.size),
            alphaMode(config.alphaMode) {}

        virtual ~SomeTexture() {}

        Vector2Int Size() const {
            return size;
        }

        Vector2 NormalOrigin() const {
            return normalOrigin;
        }

        Vector2 NormalSize() const {
            return normalSize;
        }

        Vector2 Origin() const {
            return origin;
        }

        Vector2 TrimOrigin() const {
            return trimOrigin;
        }

        Vector2Int UntrimmedSize() const {
            return untrimmedSize;
        }

        /// @return Returns the actual texture object used by the render engine
        /// In the case of a texture atlas, each child texture will have a link to its original
        /// render texture
        virtual SomeTexture* RenderTexture() const {
            return const_cast<SomeTexture*>(this);
        }

        virtual uint32_t RenderId() const {
            return renderId;
        }

        virtual void SetTextureMagnification(TextureMagnificationType value) {}

        virtual void SetTextureWrap(TextureWrapType value) {}
    };
} // namespace PJ
