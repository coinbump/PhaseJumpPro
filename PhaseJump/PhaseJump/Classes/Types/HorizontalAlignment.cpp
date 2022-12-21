#include "HorizontalAlignment.h"

using namespace PJ;

const HorizontalAlignment HorizontalAlignment::leading(std::make_shared<LeadingAligner>());
const HorizontalAlignment HorizontalAlignment::center(std::make_shared<CenterAligner>());
const HorizontalAlignment HorizontalAlignment::trailing(std::make_shared<TrailingAligner>());
