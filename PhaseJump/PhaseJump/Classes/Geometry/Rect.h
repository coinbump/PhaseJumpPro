#ifndef PJRECT_H
#define PJRECT_H

#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/18/24
 */
namespace PJ {
    struct Rect {
        Vector2 origin;
        Vector2 size;

        Rect() {}

        Rect(Vector2 origin, Vector2 size) :
            origin(origin),
            size(size) {}

        static Rect const one;

        bool operator==(Rect const& rhs) const {
            return origin == rhs.origin && size == rhs.size;
        }
    };

    struct RectInt {
        Vector2Int origin;
        Vector2Int size;

        RectInt(Vector2Int origin, Vector2Int size) :
            origin(origin),
            size(size) {}

        int Top() const {
            return origin.y;
        }

        int Bottom() const {
            return origin.y + size.x;
        }

        int Left() const {
            return origin.x;
        }

        int Right() const {
            return origin.x + size.y;
        }
    };
} // namespace PJ

#endif
