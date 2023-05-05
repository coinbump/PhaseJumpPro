#include "HorizontalAlignment.h"

using namespace PJ;

const HorizontalAlignment HorizontalAlignment::leading(MAKE<LeadingAligner>());
const HorizontalAlignment HorizontalAlignment::center(MAKE<CenterAligner>());
const HorizontalAlignment HorizontalAlignment::trailing(MAKE<TrailingAligner>());
