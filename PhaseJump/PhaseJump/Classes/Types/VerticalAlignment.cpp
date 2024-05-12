#include "VerticalAlignment.h"

using namespace PJ;

const VerticalAlignment VerticalAlignment::top(MAKE<TopAligner>());
const VerticalAlignment VerticalAlignment::center(MAKE<CenterAligner>());
const VerticalAlignment VerticalAlignment::bottom(MAKE<BottomAligner>());

VerticalAlignment VerticalAlignment::Builder::VerticalAlignmentFrom(VerticalAlignmentType type)
{
    switch (type)
    {
        case VerticalAlignmentType::Top:
            return VerticalAlignment::top;
        case VerticalAlignmentType::Bottom:
            return VerticalAlignment::bottom;
        default:
            return VerticalAlignment::center;
    }
}
