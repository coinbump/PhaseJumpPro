#pragma once

#include "Layout.h"
#include "WorldComponent.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Flow the objects with non-contextual spacing (object size doesn't
    /// matter)
    class HFlow : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = HFlow;

        Layout layout;
        float spacing = 10;

        HFlow(float spacing = 10) :
            spacing(spacing) {
            layout.applyLayoutFunc = [this](Layout&) { ApplyLayout(); };
        }

        Vector3 Size() const {
            GUARDR(owner, {})
            return Vector3(spacing * (owner->ChildNodes().size() - 1), 0, 0);
        }

        void ApplyLayout() {
            GUARD(owner)

            auto firstPos = (Size().x / 2) * vecLeft;
            auto position = firstPos;

            auto childNodes = owner->ChildNodes();
            for (auto& child : childNodes) {
                child->transform.SetLocalPosition(
                    Vector3(position, 0, child->transform.LocalPosition().z)
                );
                position += spacing;
            }
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "HFlow";
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
