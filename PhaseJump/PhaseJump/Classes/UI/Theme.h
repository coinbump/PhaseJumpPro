#pragma once

#include "Color.h"
#include "UnorderedMap.h"

// CODE REVIEW: ?/23
namespace PJ {
    /// Names of common UI elements that can be used in maps
    namespace UIElement {
        auto constexpr SliderTrack = "slider.track";
        auto constexpr SliderThumb = "slider.thumb";
        auto constexpr SliderVerticalTrack = "slider.vertical.track";
        auto constexpr SliderVerticalThumb = "slider.vertical.thumb";
        auto constexpr ButtonSurface = "button.surface";
        auto constexpr PopoverSurface = "popover.surface";
        auto constexpr ToggleSurface = "toggle.surface";
        auto constexpr ToggleSwitch = "toggle.switch";
        auto constexpr CheckboxSwitch = "checkbox.switch";
        auto constexpr CheckboxCheck = "checkbox.check";
        auto constexpr CheckboxFont = "checkbox.font";
        auto constexpr TextInputSurface = "textInput.surface";
        auto constexpr TextInputFont = "textInput.font";
        auto constexpr TabBarSurface = "tabBar.surface";
        auto constexpr ScrollBarTrack = "scrollBar.track";
        auto constexpr ScrollBarThumb = "scrollBar.thumb";
        auto constexpr ScrollBarVerticalTrack = "scrollBar.vertical.track";
        auto constexpr ScrollBarVerticalThumb = "scrollBar.vertical.thumb";
        auto constexpr ToastSurface = "toast.surface";
        auto constexpr AlertSurface = "alert.surface";
        auto constexpr SelectableItemSurface = "selectableItem.surface";
        auto constexpr SelectableItemSelectedSurface = "selectableItem.selected.surface";
        auto constexpr Dial = "dial";

        // Drag handles (for resizing/altering objects)
        auto constexpr HandleFill = "handle.fill";
        auto constexpr HandleFrame = "handle.frame";
    } // namespace UIElement

    /// Custom property names for common UI elements
    namespace UITag {
        /// Slice points for sliced texture renderer
        auto constexpr SlicePoints = "slicePoints";

        /// Size of end caps for UI objects that have one (slider)
        auto constexpr EndCapSize = "endCap.size";
    } // namespace UITag

    // Material 3 colors: https://m3.material.io/styles/color/static/baseline
    // Apple colors:
    // https://developer.apple.com/design/human-interface-guidelines/color#system-colors

    /// Used to provide swappable theme information for displaying UI
    class Theme {
    public:
        struct Element {
            String colorId;

            // FUTURE: support font, size information, etc.

            Element(String colorId) {}
        };

        // FUTURE: add font support
        using ColorMap = UnorderedMap<String, Color>;
        using SemanticColorMap = UnorderedMap<String, String>;
        using ElementMap = UnorderedMap<String, Element>;

    protected:
        /// Each color has a unique id, that must not be the same as a semantic or element color
        ColorMap colors;

        /**
         Semantic colors are mapped to colors
         Example: labelSecondary -> gray2
         Also known as "roles" in Material design
         */
        SemanticColorMap semanticColors;

        /// Element colors are mapped to either semantic or non-semantic colors
        /// Example: buttonFillPressed-> backgroundTertiary or red
        ElementMap elements;

    public:
        Theme(
            std::initializer_list<ColorMap::value_type> colors,
            std::initializer_list<SemanticColorMap::value_type> semanticColors,
            std::initializer_list<ElementMap::value_type> elements
        ) :
            colors(colors),
            semanticColors(semanticColors),
            elements(elements) {}

        ColorMap const& Colors() const {
            return colors;
        }

        Color ThemeColor(String id) const {
            try {
                id = elements.at(id).colorId;
            } catch (...) {}

            try {
                id = semanticColors.at(id);
            } catch (...) {}

            try {
                return colors.at(id);
            } catch (...) {
                return Color::black;
            }
        }
    };

    namespace Themes {
        /// Theme colors similar to a well known "fruit" company
        extern const Theme fruit;
    }; // namespace Themes
} // namespace PJ
