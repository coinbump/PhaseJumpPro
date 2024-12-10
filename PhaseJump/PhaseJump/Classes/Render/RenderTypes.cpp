#include "RenderTypes.h"

using namespace std;
using namespace PJ;

ostream& PJ::operator<<(ostream& os, RenderFeatureState const& value) {
    switch (value) {
    case RenderFeatureState::Enable:
        os << "on";
        break;
    case RenderFeatureState::Disable:
        os << "off";
        break;
    }
    return os;
}

RenderOpacityType PJ::RenderOpacityTypeFor(Color const& color) {
    return color.IsOpaque() ? RenderOpacityType::Opaque : RenderOpacityType::Blend;
}
