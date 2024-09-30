#pragma once

#include "StringUtils.h"
#include "Tags.h"
#include "UnorderedMap.h"

// CODE REVIEW: ?/23
namespace PJ {
    class WorldNode;
    class SomeWorldSystem;
    class SomeCamera;
    class RenderMaterial;
    class SomeTexture;
    class DesignSystem;
    class Font;

    struct FontSpec {
        String resourceId;
        String fontName;
        float size = 0;
    };

    /// Defines properties and methods for building a UI
    class DesignSystem {
    protected:
        /// Maps UI element ids to specific textures
        UnorderedMap<String, SP<SomeTexture>> elementTextures;

        /// Maps UI element ids to custom properties
        UnorderedMap<String, Tags> elementTags;

    public:
        void Add(String element, SP<SomeTexture> texture) {
            elementTextures.insert_or_assign(element, texture);
        }

        template <class T>
        void SetTag(String element, String tagName, T value) {
            elementTags[element].Add(tagName, value);
        }

        template <class T>
        T TagValue(String element, String tagName) const {
            try {
                auto& tags = elementTags.at(element);
                return tags.SafeValue<T>(tagName);
            } catch (...) {
                return T();
            }
        }

        SP<SomeTexture> Texture(String element) const {
            try {
                return elementTextures.at(element);
            } catch (...) {}

            return nullptr;
        }
    };

    /// Builds a UI using only meshes, no textures
    class MeshDesignSystem {
        // FUTURE: implement as needed
    };

    /// Simple, basic design system ("ugly duckling" UI)
    class DuckDesignSystem {
        // FUTURE: implement as needed
    };
} // namespace PJ
