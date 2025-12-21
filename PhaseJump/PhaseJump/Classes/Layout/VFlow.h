#pragma once

#include "Layout2D.h"
#include "Vector2.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Flow the objects with non-contextual spacing (object size doesn't
    /// matter)
    class VFlow : public Layout2D {
    public:
        float spacing{};

        VFlow(float spacing) :
            spacing(spacing) {}

        Vector3 Size() const {
            GUARDR(owner, {})

            return Vector3(0, spacing * (owner->ChildNodes().size() - 1), 0);
        }

        void ApplyLayout() override {
            GUARD(owner)

            auto firstPos = (Size().y / 2) * vecUp;
            auto position = firstPos;

            auto childNodes = owner->ChildNodes();
            for (auto& child : childNodes) {
                child->transform.SetLocalPosition(
                    Vector3(0, position, child->transform.LocalPosition().z)
                );
                position += spacing * vecDown;
            }
        }
    };
} // namespace PJ
