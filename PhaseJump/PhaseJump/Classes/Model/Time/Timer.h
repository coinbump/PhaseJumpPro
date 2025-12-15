#pragma once

#include "StringUtils.h"
#include "TimerPlayable.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/6/25
 PORTED TO: C++, Swift
 */
namespace PJ {
    /// Runs for a specified duration, then finishes or repeats
    class Timer : public SomeUpdatable {
    protected:
        /// Timer duration
        float duration{};

        /// Time state
        float state{};

        Runner runner;

    public:
        using This = Timer;
        using Base = SomeUpdatable;
        using Func = std::function<void(This&)>;

        /// Called when timer finishes
        Func onFinishFunc;

        struct Config {
            float duration;
            RunType runType;
            Func onFinishFunc;
        };

        Timer(Config const& config) :
            Timer(config.duration, config.runType, config.onFinishFunc) {}

        Timer(float duration, RunType runType, Func onFinishFunc = {}) :
            duration(duration),
            runner(runType),
            onFinishFunc(onFinishFunc) {
            runner.onFinishFunc = [this]() { this->OnFinish(); };
            runner.onResetFunc = [this]() { this->OnReset(); };
        }

        // For object copies, copy properties, not compose funcs (funcs are using captured this)
        Timer(This const& rhs) :
            duration(rhs.duration),
            runner(rhs.runner.runType) {
            runner.onFinishFunc = [this]() { this->OnFinish(); };
            runner.onResetFunc = [this]() { this->OnReset(); };
        }

        float State() const {
            return state;
        }

        void SetState(float value) {
            GUARD(state != value)
            state = std::clamp(value, 0.0f, duration);
            runner.SetIsFinished(state >= duration);
        }

        void Stop() {
            runner.runType = RunType::Once;
            runner.SetIsFinished(true);
        }

        void OnReset() {
            SetState(0);
        }

        float Progress() const {
            return std::clamp(state / duration, 0.0f, 1.0f);
        }

        void SetProgress(float progress) {
            progress = std::clamp(progress, 0.0f, 1.0f);
            SetState(progress * duration);
        }

        virtual void OnFinish() {
            GUARD(onFinishFunc);
            (onFinishFunc)(*this);
        }

        // MARK: SomeUpdatable

        void Reset() override {
            runner.Reset();
        }

        FinishType OnUpdate(TimeSlice time) override {
            if (IsFinished() || duration <= 0) {
                return FinishType::Finish;
            }

            auto delta = TimeDeltaFor(time);
            if (delta <= 0) {
                return FinishType::Continue;
            }

            // We can't break down large deltas here because events will get compressed
            // Example: an event happens every 3 seconds, we get a 9 second delta,
            // they should be 3 seconds apart but are 0 seconds apart instead
            SetState(state + delta);

            return IsFinished() ? FinishType::Finish : FinishType::Continue;
        }

        float TimeDeltaFor(TimeSlice time) {
            return time.delta;
        }

        /// Allows for cleanup of old updatables
        bool IsFinished() const override {
            return runner.IsFinished();
        }
    };
} // namespace PJ
