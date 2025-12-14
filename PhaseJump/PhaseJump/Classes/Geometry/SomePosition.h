#pragma once

#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/13/25
 */
namespace PJ {
    class WorldNode;

    /// World position. Use to clarify context for vector type
    using WorldPosition = Vector3;

    /// Local position. Use to clarify context for vector type
    using LocalPosition = Vector3;

    /// Screen position with reading coordinates.
    /// Top-left is (0, 0), bottom-right is (maxX, maxY)
    using ScreenPosition = Vector2;
} // namespace PJ
