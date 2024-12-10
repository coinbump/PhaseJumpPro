#include "VerticalAlignment.h"

using namespace PJ;

const AlignFunc VerticalAlignment::top(AlignFuncs::left);
const AlignFunc VerticalAlignment::center(AlignFuncs::center);
const AlignFunc VerticalAlignment::bottom(AlignFuncs::right);

// FUTURE: add if needed
// VerticalAlignment VerticalAlignment::Builder::VerticalAlignmentFrom(VerticalAlignmentType type) {
//    switch (type) {
//    case VerticalAlignmentType::Top:
//        return VerticalAlignment::top;
//    case VerticalAlignmentType::Bottom:
//        return VerticalAlignment::bottom;
//    default:
//        return VerticalAlignment::center;
//    }
//}
