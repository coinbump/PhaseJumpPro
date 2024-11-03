#pragma once

#include "Updatable.h"
#include "Vector3.h"
#include <functional>

// /23
// TODO: this DOES NOT WORK (need to sample multiple frames, not just 1)
namespace PJ {
    class WorldNode;

    /// Tracks velocity for an object based on updates, no samples or averages are used
    class SimpleVelocityTracker : public Updatable {
    public:
        using Base = Updatable;
        using This = SimpleVelocityTracker;

        using PositionFunc = std::function<Vector3()>;

    protected:
        std::optional<Vector3> prevPosition;
        float timeDelta{};
        Vector3 position;
        PositionFunc positionFunc;

    public:
        SimpleVelocityTracker(PositionFunc positionFunc);

        /// @return Returns the velocity of the owner node
        Vector3 Velocity() const;

        /// @return Returns the directional velocity of the owner node
        float DirectionVelocity() const {
            return Velocity().Magnitude();
        }
    };
} // namespace PJ
