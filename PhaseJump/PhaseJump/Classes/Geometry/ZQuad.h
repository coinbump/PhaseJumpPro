#pragma once

#include "Vector3.h"
#include <array>
#include <stdio.h>

/*
 RATING: 4 stars
 Has unit tests
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    /// Stores quad vertices in a Z shape: top-left, top-right, bottom-left,
    /// bottom-right. Useful for rendering triangle strips
    struct ZQuad {
        using This = ZQuad;

        static constexpr size_t size = 4;

        std::array<Vector3, size> value;

        Vector3* data() noexcept {
            return value.data();
        }

        const Vector3* data() const noexcept {
            return value.data();
        }

        Vector3 const& ModGet(size_t index) const {
            return value[index >= 0 && index < size ? index : index % size];
        }

        Vector3& ModGet(size_t index) {
            return value[index >= 0 && index < size ? index : index % size];
        }

        static int VertexIndexToCornerIndex(int index);
        static int CornerIndexToVertexIndex(int index);

        void FlipH();
        void FlipV();
        void Turn(int count);
        void Offset(Vector3 offset);

        constexpr bool operator==(This const& rhs) const {
            return value == rhs.value;
        }
    };
} // namespace PJ
