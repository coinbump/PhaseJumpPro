#pragma once

#include "AnimationCycleTypes.h"
#include "Playable.h"
#include "PlayableTimer.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Handles animation cycle logic for an animation
    /// Alternatively, ping pong animations can be accomplished by turning a valve on and off
    class AnimationCycleTimer : public Playable {
    public:
        using Base = Playable;
        using This = AnimationCycleTimer;
        using OnCycleFunc = std::function<void(This&)>;

    protected:
        PlayableTimer timer;

        AnimationCycleType cycleType;
        AnimationCycleState cycleState;

    public:
        /// Called when the timer starts a new animation cycle
        OnCycleFunc onCycleFunc;

        AnimationCycleTimer(float duration, AnimationCycleType cycleType);

        virtual ~AnimationCycleTimer() {};

        float CycleTime() const {
            return timer.duration;
        }

        void SetCycleTime(float value) {
            timer.duration = value;
        }

        AnimationCycleState CycleState() const {
            return cycleState;
        }

        void SetCycleState(AnimationCycleState value) {
            if (cycleState == value) {
                return;
            }
            cycleState = value;
        }

        AnimationCycleType CycleType() const {
            return cycleType;
        }

        void SetCycleType(AnimationCycleType value) {
            if (cycleType == value) {
                return;
            }
            cycleType = value;
        }

        bool IsReversed() const {
            return cycleState == AnimationCycleState::Reverse;
        }

        // MARK: Updatable

        FinishType OnUpdate(TimeSlice time) override;

        // MARK: - Playable

        void Reset() override;
        float PlayTime() const override;
        void SetPlayTime(float value) override;
        float Progress() const override;
        void SetProgress(float value) override;
    };
} // namespace PJ
