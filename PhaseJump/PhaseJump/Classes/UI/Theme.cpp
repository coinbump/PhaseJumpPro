#include "Theme.h"

using namespace std;
using namespace PJ;

// FUTURE: support theme files (CSV or JSON)
// FUTURE: support dark mode
Theme const Themes::fruit{ Theme::Config{ .colors = {
                                              { "red", Color32(255, 59, 48) },
                                              { "orange", Color32(255, 149, 0) },
                                              { "yellow", Color32(255, 204, 0) },
                                              { "green", Color32(52, 199, 89) },
                                              { "mint", Color32(0, 199, 190) },
                                              { "teal", Color32(48, 176, 199) },
                                              { "cyan", Color32(50, 173, 230) },
                                              { "blue", Color32(0, 122, 255) },
                                              { "indigo", Color32(88, 86, 214) },
                                              { "purple", Color32(175, 82, 222) },
                                              { "pink", Color32(255, 45, 85) },
                                              { "brown", Color32(162, 132, 94) },
                                              { "themeGray", Color32(142, 142, 147) },
                                              { "themeGray2", Color32(174, 174, 178) },
                                              { "themeGray3", Color32(199, 199, 204) },
                                              { "themeGray4", Color32(209, 209, 214) },
                                              { "themeGray5", Color32(229, 229, 234) },
                                              { "themeGray6", Color32(242, 242, 247) },
                                          } } };

Theme::Theme(Config const& config) :
    colors(config.colors) {

    for (auto i : config.elements) {
        auto elementId = i.first;
        auto& elementConfig = i.second;

        if (!IsEmpty(elementConfig.alias)) {
            SetElementTag(elementId, "alias", elementConfig.alias);
            continue;
        }

        if (!IsEmpty(elementConfig.colorId)) {
            SetElementTag(elementId, "colorId", elementConfig.colorId);
        }
        if (elementConfig.size) {
            SetElementTag(elementId, "size", *elementConfig.size);
        }
        if (elementConfig.padding) {
            SetElementTag(elementId, "padding", *elementConfig.padding);
        }
        if (elementConfig.spacing) {
            SetElementTag(elementId, "spacing", *elementConfig.spacing);
        }
        if (elementConfig.fontSpec) {
            SetElementTag(elementId, "fontSpec", *elementConfig.fontSpec);
        }
    }
}

String Theme::ResolveElementId(String id) const {
    auto alias = ElementTagValue<String>(id, "alias");
    if (!IsEmpty(alias)) {
        return alias;
    }

    return id;
}

Color Theme::ThemeColor(String id, Color defaultValue) const {
    String colorId;

    id = ResolveElementId(id);

    // Find the element/semantic color id, if any
    try {
        auto elementColorId = ElementTagValueAt<String>(id, "colorId");
        return colors.at(elementColorId);
    } catch (...) {}

    // Return the base color, based in id
    try {
        return colors.at(id);
    } catch (...) {}

    return defaultValue;
}

Vector2 Theme::ElementSize(String id, Vector2 defaultValue) const {
    id = ResolveElementId(id);

    try {
        auto elementSize = ElementTagValueAt<Vector2>(id, "size");
        return elementSize;
    } catch (...) {}

    return defaultValue;
}
