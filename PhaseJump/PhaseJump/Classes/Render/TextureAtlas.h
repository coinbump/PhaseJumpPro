#pragma once

#include "AtlasTexture.h"
#include "Base.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/10/24
 */
namespace PJ {
    /// Texture atlas resource
    class TextureAtlas : public Base {
    public:
        using TextureList = VectorList<SP<AtlasTexture>>;

    protected:
        TextureList textures;
        UnorderedMap<String, SP<AtlasTexture>> textureMap;

    public:
        /// Converts texture collection type
        VectorList<SP<SomeTexture>> SomeTextures() {
            VectorList<SP<SomeTexture>> result;
            std::transform(
                textures.begin(), textures.end(), std::back_inserter(result),
                [](SP<AtlasTexture> texture) { return texture; }
            );

            return result;
        }

        TextureList& Textures() {
            return textures;
        }

        TextureList const& Textures() const {
            return textures;
        }

        void Add(SP<AtlasTexture> texture) {
            GUARD(texture);

            textures.push_back(texture);
            textureMap[texture->id] = texture;
        }

        Vector2Int MaxTextureTrueSize() {
            Vector2Int result = Vector2Int::zero;

            std::for_each(textures.begin(), textures.end(), [&](SP<AtlasTexture>& texture) {
                result.x = std::max(result.x, texture->trueSize.x);
                result.y = std::max(result.y, texture->trueSize.y);
            });

            return result;
        }
    };
} // namespace PJ
