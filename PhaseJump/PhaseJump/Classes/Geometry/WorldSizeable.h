#pragma once

#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/25/23
 */
namespace PJ {
    /// Interface to an object that can return its world size
    class WorldSizeable {
    public:
        virtual ~WorldSizeable() {}

        // TODO: why is this optional? Doesn't everything have a size?
        virtual std::optional<Vector3> WorldSize() const {
            return std::nullopt;
        }

        virtual void SetWorldSize(Vector3 value) = 0;
    };
} // namespace PJ
