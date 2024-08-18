#include "RenderTypes.h"

using namespace std;
using namespace PJ;

ostream& PJ::operator<<(ostream& os, RenderFeatureStatus const& value) {
    switch (value) {
    case RenderFeatureStatus::Enable:
        os << "on";
        break;
    case RenderFeatureStatus::Disable:
        os << "off";
        break;
    }
    return os;
}
