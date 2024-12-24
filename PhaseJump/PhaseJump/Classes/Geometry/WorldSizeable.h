#pragma once

#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/15/24
 */
namespace PJ {
    /// Interface to an object that can return its world size
    class WorldSizeable {
    public:
        virtual ~WorldSizeable() {}

        /// @return Returns the size of this object in world coordinates
        virtual Vector3 WorldSize() const = 0;

        /// Sets the size of this object in world coordinates
        virtual void SetWorldSize(Vector3 value) = 0;
    };
} // namespace PJ
