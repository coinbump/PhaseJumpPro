#pragma once

#include "Dev.h"
#include "Macros.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    /// Handles life state for a part of the world
    class WorldPartLife {
    private:
        /// If >0, Awake/Start or both have been called for this node (1: Awake, 2:
        /// both Awake and Start)
        int awakeCount = 0;

        void Awake() {
            awakeCount++;
        }

        void Start() {
            awakeCount++;
        }

    public:
        using LifeFunc = std::function<void()>;

        bool IsAwake() const {
            return awakeCount > 0;
        }

        bool IsStarted() const {
            return awakeCount > 1;
        }

        void CheckedAwake(LifeFunc awakeFunc) {
            GUARD(!IsAwake());
            Awake();

            GUARD(awakeFunc)
            awakeFunc();
        }

        void CheckedStart(LifeFunc startFunc) {
            GUARD_LOG(IsAwake(), "ERROR. Call Awake before Start")

            GUARD(!IsStarted());
            Start();

            GUARD(startFunc)
            startFunc();
        }
    };
} // namespace PJ
