#ifndef PJZQUAD_H
#define PJZQUAD_H

#include "Vector3.h"
#include <stdio.h>
#include <array>

/*
 RATING: 4 stars
 Could use unit tests
 CODE REVIEW: 4/13/23
 */
namespace PJ {
    /// Arranges quad vertices in a Z shape: top-left, top-right, bottom-left, bottom-right
    /// Useful for rendering triangle strips
    class ZQuad : public std::array<Vector3, 4>
    {
    public:
        static int VertexIndexToCornerIndex(int index);
        static int CornerIndexToVertexIndex(int index);

        void FlipH();
        void FlipV();
        void TurnRight(int count);
        void TurnLeft(int count);
        void Offset(Vector3 offset);
    };
}

#endif
