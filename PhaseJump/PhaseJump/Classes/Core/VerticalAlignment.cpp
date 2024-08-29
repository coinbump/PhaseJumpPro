#include "VerticalAlignment.h"

using namespace PJ;

const VerticalAlignment VerticalAlignment::top(AlignFuncs::left);
const VerticalAlignment VerticalAlignment::center(AlignFuncs::center);
const VerticalAlignment VerticalAlignment::bottom(AlignFuncs::right);

VerticalAlignment VerticalAlignment::Builder::VerticalAlignmentFrom(VerticalAlignmentType type) {
    switch (type) {
    case VerticalAlignmentType::Top:
        return VerticalAlignment::top;
    case VerticalAlignmentType::Bottom:
        return VerticalAlignment::bottom;
    default:
        return VerticalAlignment::center;
    }
}
