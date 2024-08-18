#ifndef PJZQUAD_H
#define PJZQUAD_H

#include "Vector3.h"
#include <array>
#include <stdio.h>

/*
 RATING: 4 stars
 TODO: needs unit tests
 CODE REVIEW: 4/13/23
 */
namespace PJ {
    /// Arranges quad vertices in a Z shape: top-left, top-right, bottom-left,
    /// bottom-right Useful for rendering triangle strips
    struct ZQuad {
        static constexpr size_t size = 4;

        std::array<Vector3, size> value;

        Vector3* data() noexcept {
            return value.data();
        }

        const Vector3* data() const noexcept {
            return value.data();
        }

        Vector3 const& operator[](size_t index) const {
            return value[index >= 0 && index < size ? index : index % size];
        }

        Vector3& operator[](size_t index) {
            return value[index >= 0 && index < size ? index : index % size];
        }

        static int VertexIndexToCornerIndex(int index);
        static int CornerIndexToVertexIndex(int index);

        void FlipH();
        void FlipV();
        void TurnRight(int count);
        void TurnLeft(int count);
        void Offset(Vector3 offset);
    };
} // namespace PJ

#endif
