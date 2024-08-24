#ifndef PJVFLOW_H
#define PJVFLOW_H

#include "SomeLayout2D.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Flow the objects with non-contextual spacing (object size doesn't
    /// matter)
    class VFlow : public SomeLayout2D {
    public:
        float spacing = 1.0f;

        VFlow(float spacing = 1.0f) :
            spacing(spacing) {}

        Vector3 Size() const override {
            return Vector3(0, spacing * (ChildNodes().size() - 1), 0);
        }

        void ApplyLayout() override {
            auto firstPos = (Size().y / 2) * Vector2::up.y;
            auto position = firstPos;

            auto childNodes = ChildNodes();
            for (auto& child : childNodes) {
                child->transform.SetLocalPosition(
                    Vector3(0, position, child->transform.LocalPosition().z)
                );
                position += spacing * Vector2::down.y;
            }
        }
    };
} // namespace PJ

#endif
