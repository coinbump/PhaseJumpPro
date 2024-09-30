#pragma once

#include "StringUtils.h"
#include "TimerPlayable.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Runs for N seconds, then triggers OnFinish
    class Timer : public TimerPlayable {
    protected:
        /// Time state
        float state = 0;

    public:
        using Base = TimerPlayable;

        Timer(float duration, RunType runType) :
            Base(duration, runType) {}

        // Copy properties, not compose funcs (storing captured this)
        Timer(Timer const& rhs) :
            Base(rhs.duration, rhs.runner.runType) {}

        float State() const {
            return state;
        }

        void SetState(float value) {
            GUARD(state != value)
            state = std::clamp(value, 0.0f, duration);
            runner.SetIsFinished(state >= duration);

            OnPlayTimeChange();
        }

        void OnReset() override {
            TimerPlayable::OnReset();

            SetState(0);
        }

        // MARK: Playable

        float Progress() const override {
            return std::clamp(state / duration, 0.0f, 1.0f);
        }

        float PlayTime() const override {
            return state;
        }

        void SetPlayTime(float time) override {
            SetState(time);
        }

        void SetProgress(float progress) override {
            progress = std::clamp(progress, 0.0f, 1.0f);
            SetState(progress * duration);
        }

        void OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            if (IsFinished() || duration <= 0) {
                return;
            }

            auto delta = TimeDeltaFor(time);
            if (delta <= 0) {
                return;
            }

            // We can't break down large deltas because events will get compressed
            // Example: an event happens every 3 seconds, we get a 9 second delta,
            // they should be 3 seconds apart but are 0 seconds apart instead
            // Need a different mechanism to support fast forward in sims
            SetState(state + delta);
        }
    };
} // namespace PJ
