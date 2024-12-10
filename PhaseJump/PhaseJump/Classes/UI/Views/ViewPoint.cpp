#include "ViewPoint.h"

using namespace std;
using namespace PJ;

const ViewPoint ViewPoint::center{ .x = 0.5f, .y = 0.5f };
const ViewPoint ViewPoint::leading{ .x = 0, .y = 0.5f };
const ViewPoint ViewPoint::trailing{ .x = 1, .y = 0.5f };
const ViewPoint ViewPoint::topLeading{ .x = 0, .y = 0 };
const ViewPoint ViewPoint::topTrailing{ .x = 1, .y = 0 };
const ViewPoint ViewPoint::bottomLeading{ .x = 0, .y = 1 };
const ViewPoint ViewPoint::bottomTrailing{ .x = 1, .y = 1 };
