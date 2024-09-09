#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    // A UVRect has flipped coordinates from Rect
    // [0,0] is the bottom left. [1,1] is the top-right
    // For repeat textures, the UV value can extend past [1,1]
    struct UVRect {
        Vector2 origin;
        Vector2 size;

        constexpr UVRect() {}

        constexpr UVRect(Vector2 origin, Vector2 size) :
            origin(origin),
            size(size) {}

        bool operator==(UVRect const& rhs) const {
            return this->origin == rhs.origin && this->size == rhs.size;
        }
    };

    static constexpr UVRect uvRectOne{ { 0, 0 }, { 1, 1 } };
} // namespace PJ
