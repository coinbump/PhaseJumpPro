#pragma once

#include "Color.h"
#include "Font.h"
#include "UITypes.h"
#include "UnorderedMap.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    /// Provides theme metadata for displaying UI
    class Theme {
    public:
        /*
         REFERENCE:
         Google
         Material 3 colors: https://m3.material.io/styles/color/static/baseline
         Material 3 roles: https://m3.material.io/styles/color/roles

         Apple
         Apple colors:
         https://developer.apple.com/design/human-interface-guidelines/color#system-colors
         */

        // FUTURE: add font support
        using ColorMap = UnorderedMap<String, Color>;

    protected:
        /// Each color has a unique id, that must not be the same as a semantic or element color
        ColorMap colors;

        /// Maps UI element ids to custom properties
        Storage<> elements;

        String ResolveElementId(String id) const;

    public:
        /// Metadata for a configuring UI elements
        struct ElementConfig {
            /// Alias to another element
            String alias;

            String colorId;

            /// Layout size for this element
            std::optional<Vector2> size;

            /// Layout padding for this element
            std::optional<Vector2> padding;

            /// Layout spacing for this element
            std::optional<Vector2> spacing;

            /// Font information for this element
            std::optional<FontSpec> fontSpec;
        };

        struct Config {
            using ElementConfigMap = UnorderedMap<String, ElementConfig>;

            ColorMap colors;
            ElementConfigMap elements;
        };

        Theme(Config const& config);

        ColorMap const& Colors() const {
            return colors;
        }

        /// Sets a tag value for a UI element
        template <class Type>
        void SetElementTag(String elementId, String tagName, Type value) {
            elements.Set(elementId, tagName, value);
        }

        /// @return Returns a tag value for a UI element
        template <class Type>
        Type ElementTagValue(String elementId, String tagName, Type defaultValue = {}) const {
            return elements.Value<Type>(elementId, tagName, defaultValue);
        }

        /// @return Returns value if it exists
        /// @throws Throws error if value doesn't exist
        template <class Type>
        Type ElementTagValueAt(String elementId, String tagName) const {
            return elements.At<Type>(elementId, tagName);
        }

        /// @return Returns the color for the specified color id
        Color ThemeColor(String id, Color defaultValue = Color::black) const;

        /// @return Returns the size for the specified element id
        Vector2 ElementSize(String id, Vector2 defaultValue = {}) const;

        /// @return Returns the spacing for the specified element id
        Vector2 ElementSpacing(String id, Vector2 defaultValue = {}) const;
    };

    namespace Themes {
        /// Theme colors similar to a well known "fruit" company
        extern const Theme fruit;
    }; // namespace Themes
} // namespace PJ
