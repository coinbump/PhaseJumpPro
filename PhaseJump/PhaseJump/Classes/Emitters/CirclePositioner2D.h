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
    /// Positions objects in a circle
    namespace CirclePositioner2D {
        PositionFunc MakeFunc(float radius);
    }; // namespace CirclePositioner2D
} // namespace PJ
