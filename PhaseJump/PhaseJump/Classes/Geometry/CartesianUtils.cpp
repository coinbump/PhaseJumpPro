#include "CartesianUtils.h"

using namespace std;
using namespace PJ;

Rect CartesianUtils::CenteredIn(Rect value, Vector2 outerSize) {
    auto result = value;

    result.origin = { outerSize.x * 0.5f * vecRight + result.size.x * 0.5f * vecLeft,
                      outerSize.y * 0.5f * vecUp + result.size.y * 0.5f * vecDown };

    return result;
}
