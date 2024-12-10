#pragma once

#include "WorldComponent.h"
#include "WorldSizeable.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Colliders are attached to object to define region for collisions and hit
    /// testing
    class SomeCollider : public WorldComponent<>, public WorldSizeable {};
} // namespace PJ
