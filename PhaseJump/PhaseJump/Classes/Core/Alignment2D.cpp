#include "Alignment2D.h"

using namespace PJ;

const Alignment2D Alignment2D::topLeading(AlignFuncs::left, AlignFuncs::left);
const Alignment2D Alignment2D::top(AlignFuncs::center, AlignFuncs::left);
const Alignment2D Alignment2D::topTrailing(AlignFuncs::right, AlignFuncs::left);
const Alignment2D Alignment2D::leading(AlignFuncs::left, AlignFuncs::center);
const Alignment2D Alignment2D::center{ AlignFuncs::center, AlignFuncs::center };
const Alignment2D Alignment2D::trailing(AlignFuncs::right, AlignFuncs::center);
const Alignment2D Alignment2D::bottomLeading(AlignFuncs::left, AlignFuncs::right);
const Alignment2D Alignment2D::bottom(AlignFuncs::center, AlignFuncs::right);
const Alignment2D Alignment2D::bottomTrailing(AlignFuncs::right, AlignFuncs::right);
