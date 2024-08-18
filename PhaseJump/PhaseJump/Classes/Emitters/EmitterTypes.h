#pragma once

#include "Function.h"
#include "SomeRandom.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/15/24
 */
namespace PJ {
    /// Determines the position of a newly spawned object
    using PositionFunc = std::function<Vector3(SomeRandom&)>;
} // namespace PJ
