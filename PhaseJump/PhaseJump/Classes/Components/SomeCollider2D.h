#pragma once

#include "SomeCollider.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// 2D collider
    class SomeCollider2D : public SomeCollider {
    public:
        virtual bool TestHit(Vector2 position) = 0;
    };
} // namespace PJ
