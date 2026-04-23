#include "MaterialRenderModel.h"

using namespace std;
using namespace PJ;

bool MaterialRenderModel::IsFeatureEnabled(String feature) const {
    // Model overrides win over the material's state
    auto overrideIt = overrideFeatures.find(feature);
    if (overrideIt != overrideFeatures.end()) {
        return overrideIt->second == RenderFeatureState::Enable;
    }

    GUARDR(material, false)

    // FUTURE: support feature defaults if needed
    auto i = material->Features().find(feature);
    GUARDR(i != material->Features().end(), false)

    return i->second == RenderFeatureState::Enable;
}
