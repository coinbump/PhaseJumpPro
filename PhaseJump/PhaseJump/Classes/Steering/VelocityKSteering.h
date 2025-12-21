#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/14/24
 */
namespace PJ {
    /// Kinematic steering. Moves node at a specific velocity
    class VelocityKSteering : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = VelocityKSteering;

        Vector3 velocity;

        VelocityKSteering(Vector3 velocity = {}) :
            velocity(velocity) {}

        // MARK: SomeWorldComponent

        FinishType OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            GUARDR(owner, FinishType::Continue)

            auto moveDelta = velocity * time.delta;
            owner->transform.SetLocalPosition(owner->transform.LocalPosition() + moveDelta);

            return FinishType::Continue;
        }

        String TypeName() const override {
            return "VelocityKSteering";
        }
    };
} // namespace PJ
