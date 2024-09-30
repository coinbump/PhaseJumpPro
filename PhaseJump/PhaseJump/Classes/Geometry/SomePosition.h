#pragma once

#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    // FUTURE: needs conversion logic
    class WorldNode;

    /// World position. Use when Vector type doesn't make the context clear
    using WorldPosition = Vector3;

    /// Local position. Use when Vector type doesn't make the context clear
    using LocalPosition = Vector3;

    /// Screen position. Use when Vector type doesn't make the context clear
    using ScreenPosition = Vector2;
} // namespace PJ
