#pragma once

#include "Layout.h"
#include "Vector2.h"
#include "WorldComponent.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Flow the objects with non-contextual spacing (object size doesn't
    /// matter)
    class VFlow : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = VFlow;

        Layout layout;
        float spacing{};

        VFlow(float spacing) :
            spacing(spacing) {
            layout.applyLayoutFunc = [this](Layout&) { ApplyLayout(); };
        }

        Vector3 Size() const {
            GUARDR(owner, {})

            return Vector3(0, spacing * (owner->ChildNodes().size() - 1), 0);
        }

        void ApplyLayout() {
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

    protected:
        // MARK: WorldComponent

        void Awake() override {
            Base::Awake();
            layout.Awake(*this);
        }

        void Start() override {
            Base::Start();
            layout.Start(*this);
        }
    };
} // namespace PJ
