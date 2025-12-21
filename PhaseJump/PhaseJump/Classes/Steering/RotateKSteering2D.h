#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// Rotates the node N angles per second
    class RotateKSteering2D : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = RotateKSteering2D;

        Angle turnSpeed;

        RotateKSteering2D(Angle turnSpeed) :
            turnSpeed(turnSpeed) {}

        void TurnLeft(Angle value) {
            turnSpeed = -value;
        }

        void Turn(Angle value) {
            turnSpeed = value;
        }

        // MARK: SomeWorldComponent

        FinishType OnUpdate(TimeSlice time) override;

        String TypeName() const override {
            return "RotateKSteering2D";
        }
    };
} // namespace PJ
