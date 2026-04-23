#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/16/24
 */
namespace PJ {
    /// Stores bounds information where origin is the origin of the frame
    struct Rect {
        using This = Rect;

        Vector2 origin;
        Vector2 size;

        static Rect const one;

        bool operator==(This const&) const = default;
    };

    // MARK: RectInt

    /// Stores integer bounds information in reading space (cartesian coordinates are not supported)
    struct RectInt {
        using This = RectInt;

        Vector2Int origin;
        Vector2Int size;

        int Top() const {
            return origin.y;
        }

        int Left() const {
            return origin.x;
        }

        int Bottom() const {
            return origin.y + size.y - 1;
        }

        int Right() const {
            return origin.x + size.x - 1;
        }

        bool operator==(This const&) const = default;
    };
} // namespace PJ
