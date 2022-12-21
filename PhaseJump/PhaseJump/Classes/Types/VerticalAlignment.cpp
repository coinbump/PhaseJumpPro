#include "VerticalAlignment.h"

using namespace PJ;

const VerticalAlignment VerticalAlignment::top(std::make_shared<TopAligner>());
const VerticalAlignment VerticalAlignment::center(std::make_shared<CenterAligner>());
const VerticalAlignment VerticalAlignment::bottom(std::make_shared<BottomAligner>());
