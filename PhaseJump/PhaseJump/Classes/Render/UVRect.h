#ifndef PJUVRECT_H
#define PJUVRECT_H

#include "Vector2.h"

namespace PJ
{
    // A UVRect has flipped coordinates from Rect
    // [0,0] is the bottom left. [1,1] is the top-right
    // For repeat textures, the UV value can extend past [1,1]
    struct UVRect {
        Vector2 origin;
        Vector2 size;

        UVRect() {
        }

        UVRect(Vector2 origin, Vector2 size) : origin(origin), size(size) {
        }

        bool operator==(UVRect const& rhs) const { return this->origin == rhs.origin && this->size == rhs.size; }

        static UVRect const one;
    };
}

#endif
