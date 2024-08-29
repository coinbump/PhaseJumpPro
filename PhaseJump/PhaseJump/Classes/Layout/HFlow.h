#pragma once

#include "SomeLayout2D.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Flow the objects with non-contextual spacing (object size doesn't
    /// matter)
    class HFlow : public SomeLayout2D {
    public:
        float spacing = 1.0f;

        HFlow(float spacing = 1.0f) :
            spacing(spacing) {}

        Vector3 Size() const override {
            GUARDR(owner, Vector3::zero)
            return Vector3(spacing * (owner->ChildNodes().size() - 1), 0, 0);
        }

        void ApplyLayout() override {
            GUARD(owner)

            auto firstPos = -Size().x / 2;
            auto position = firstPos;

            auto childNodes = owner->ChildNodes();
            for (auto& child : childNodes) {
                child->transform.SetLocalPosition(
                    Vector3(position, 0, child->transform.LocalPosition().z)
                );
                position += spacing;
            }
        }
    };
} // namespace PJ
