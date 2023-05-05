#include "VerticalAlignment.h"

using namespace PJ;

const VerticalAlignment VerticalAlignment::top(MAKE<TopAligner>());
const VerticalAlignment VerticalAlignment::center(MAKE<CenterAligner>());
const VerticalAlignment VerticalAlignment::bottom(MAKE<BottomAligner>());
