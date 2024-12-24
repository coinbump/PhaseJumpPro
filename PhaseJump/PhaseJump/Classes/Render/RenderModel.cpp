#include "RenderModel.h"

using namespace std;
using namespace PJ;

bool RenderModel::IsFeatureEnabled(String feature) const {
    GUARDR(material, false)

    // FUTURE: support feature defaults if needed
    auto i = material->Features().find(feature);
    GUARDR(i != material->Features().end(), false)

    return i->second == RenderFeatureState::Enable;
}
