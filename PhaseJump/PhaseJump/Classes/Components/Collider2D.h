#pragma once

#include "Collider.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    /// 2D collider
    class Collider2D : public Collider {
    public:
        using TestHitFunc = std::function<bool(Vector2 position)>;

        TestHitFunc testHitFunc;

        virtual bool TestHit(Vector2 position) {
            GUARDR(testHitFunc, false);
            return testHitFunc(position);
        }
    };
} // namespace PJ
