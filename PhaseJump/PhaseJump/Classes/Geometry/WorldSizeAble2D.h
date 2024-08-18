#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    class WorldSizeAble2D {
    public:
        virtual ~WorldSizeAble2D() {}

        virtual Vector2 WorldSize2D() const = 0;
        virtual void SetWorldSize2D(Vector2 value) = 0;
    };
} // namespace PJ
