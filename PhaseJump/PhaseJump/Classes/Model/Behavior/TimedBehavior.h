#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/16/24
 */
namespace PJ {
    /// Runs a behavior for a specific amount of time
    class TimedBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = TimedBehavior;
        using TimeValueFunc = std::function<float()>;

    protected:
        /// Seconds left in timer
        float time{};
        float timeMax{};

    public:
        /// Returns time value in seconds
        TimeValueFunc timeValueFunc;

        TimedBehavior(TimeValueFunc timeValueFunc);
        TimedBehavior(float time);

        float TimeMax() const {
            return timeMax;
        }

        float TimeRemainder() const {
            return time;
        }

        // MARK: BehaviorNode

        void OnRun() override;
    };
} // namespace PJ
