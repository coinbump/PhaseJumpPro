#include "LayoutTypes.h"
#include "AlignFunc.h"

using namespace std;
using namespace PJ;

auto constexpr startValue = 0;
auto constexpr centerValue = 0.5f;
auto constexpr endValue = 1.0f;

const LayoutAnchor2D LayoutAnchor2D::topLeft(startValue, startValue);
const LayoutAnchor2D LayoutAnchor2D::top(centerValue, startValue);
const LayoutAnchor2D LayoutAnchor2D::topRight(endValue, startValue);
const LayoutAnchor2D LayoutAnchor2D::left(startValue, centerValue);
const LayoutAnchor2D LayoutAnchor2D::center{ centerValue, centerValue };
const LayoutAnchor2D LayoutAnchor2D::right(endValue, centerValue);
const LayoutAnchor2D LayoutAnchor2D::bottomLeft(startValue, endValue);
const LayoutAnchor2D LayoutAnchor2D::bottom(centerValue, endValue);
const LayoutAnchor2D LayoutAnchor2D::bottomRight(endValue, endValue);
