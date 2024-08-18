#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/16/24
 */
namespace PJ {
    /// Colliders are attached to object to define region for collisions and hit
    /// testing
    class SomeCollider : public WorldComponent<> {};
} // namespace PJ
