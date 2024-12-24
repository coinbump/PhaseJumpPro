#pragma once

#include "Updatable.h"
#include "ValueSampler.h"
#include "Vector3.h"
#include <functional>

// Future Feature, Unfinished, don't use
namespace PJ {
    class WorldNode;

    /// Tracks velocity for an object based on updates, no samples or averages are used
    class VelocityTracker : public Updatable {
    public:
        using Base = Updatable;
        using This = VelocityTracker;

        using PositionFunc = std::function<Vector3()>;

    protected:
        ValueSampler<Vector3> sampler;
        PositionFunc positionFunc;

    public:
        VelocityTracker(PositionFunc positionFunc);

        /// @return Returns the velocity of the owner node
        Vector3 Velocity() const;

        /// @return Returns the directional velocity of the owner node
        float DirectionVelocity() const {
            return Velocity().Magnitude();
        }
    };
} // namespace PJ
