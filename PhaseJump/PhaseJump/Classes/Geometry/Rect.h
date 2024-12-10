#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/16/24
 */
namespace PJ {
    /// Stores bounds information in reading space
    struct Rect {
        using This = Rect;

        Vector2 origin;
        Vector2 size;

        static Rect const one;

        bool operator==(This const& rhs) const {
            return origin == rhs.origin && size == rhs.size;
        }
    };

    // MARK: RectInt

    /// Stores integer bounds information in reading space
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
            return origin.y + size.x - 1;
        }

        int Right() const {
            return origin.x + size.y - 1;
        }

        bool operator==(This const& rhs) const {
            return origin == rhs.origin && size == rhs.size;
        }
    };
} // namespace PJ
