#pragma once

#include "WorldComponent.h"
#include "WorldSizeable.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    /// Colliders are attached to object to define region for collisions and hit
    /// testing
    class Collider : public WorldComponent, public WorldSizeable {};
} // namespace PJ
