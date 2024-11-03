#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// Rotates the node N angles per second
    class RotateKSteering2D : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = RotateKSteering2D;

        Angle turnSpeed;

        RotateKSteering2D(Angle turnSpeed) :
            turnSpeed(turnSpeed) {}

        void TurnLeft(Angle value) {
            turnSpeed = -value;
        }

        void TurnRight(Angle value) {
            turnSpeed = value;
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;

        // MARK: SomeWorldComponent

        virtual String TypeName() const override {
            return "RotateKSteering2D";
        }
    };
} // namespace PJ
