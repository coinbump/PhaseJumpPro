#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// Kinematic steering. Moves node in the direction it is facing
    class DirectionKSteering2D : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = DirectionKSteering2D;

        float velocity = 10;
        float acceleration{};
        float maxVelocity{};

        DirectionKSteering2D(float velocity = 10, float acceleration = 0, float maxVelocity = 0);

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;
    };
} // namespace PJ
