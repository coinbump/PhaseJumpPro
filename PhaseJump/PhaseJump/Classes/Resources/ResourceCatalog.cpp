#include "ResourceCatalog.h"

using namespace std;
using namespace PJ;

SP<Font> PJ::FindFontWithSize(ResourceCatalog const& resources, float size) {
    FontSpec spec{ .size = size };
    return FindFont(resources, spec);
}

SP<Font> PJ::FindFontWithResourceId(ResourceCatalog const& resources, String id) {
    FontSpec spec{ .resourceId = id };
    return FindFont(resources, spec);
}

SP<Font> PJ::FindFont(ResourceCatalog const& resources, FontSpec spec) {
    try {
        auto& typeMap = resources.Map().at("font");
        if (spec.resourceId.size() > 0) {
            auto result = typeMap.at(spec.resourceId);
            return DCAST<Font>(result.resource);
        }

        VectorList<SP<Font>> candidates;
        for (auto const& item : typeMap) {
            auto font = DCAST<Font>(item.second.resource);
            GUARD_CONTINUE(font)
            candidates.push_back(font);
        }

        GUARDR(!IsEmpty(candidates), nullptr)

        std::sort(
            candidates.begin(), candidates.end(),
            [&](SP<Font> const& lhs, SP<Font> const& rhs) {
                auto Weight = [](FontSpec spec, SP<Font> const& font) {
                    int result = 0;
                    if (!IsEmpty(spec.fontName)) {
                        if (font->name == spec.fontName) {
                            result++;
                        }
                    }
                    if (spec.size > 0) {
                        if (font->size == spec.size) {
                            result += 2;
                        }
                    }

                    return result;
                };

                int leftWeight = Weight(spec, lhs);
                int rightWeight = Weight(spec, rhs);

                return leftWeight > rightWeight;
            }
        );

        return candidates[0];
    } catch (...) {
        return {};
    }
}
