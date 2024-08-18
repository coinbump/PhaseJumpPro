#pragma once

#include "Angle.h"
#include "EmitterTypes.h"
#include "SomeProducer.h"
#include "SomeRandom.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/15/24
 */
namespace PJ {
    /// Positions objects in a rect
    namespace RectPositioner2D {
        PositionFunc MakeFunc(Vector2 worldSize);
    }; // namespace RectPositioner2D
} // namespace PJ
