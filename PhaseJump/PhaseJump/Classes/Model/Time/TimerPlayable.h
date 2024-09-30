#pragma once

#include "Base.h"
#include "Playable.h"
#include "Runner.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests (via Timer)
 CODE REVIEW: 7/7/24
 */
namespace PJ {
    /// A timer-type playable with run-once or run-repeat behavior
    class TimerPlayable : public Playable {
    public:
        using Base = Playable;
        using This = TimerPlayable;
        using RunType = Runner::RunType;

        /// Duration of this playable
        float duration = 0;

        /// Modifies time scale for update events
        float timeScale = 1.0f;

        Runner runner;

        std::function<void(This&)> onFinishFunc;

        TimerPlayable(float duration, RunType runType) :
            runner(runType),
            duration(duration) {
            runner.onFinishFunc = [this](Runner& runner) { this->OnFinish(); };
            runner.onResetFunc = [this](Runner& runner) { this->OnReset(); };
        }

        virtual ~TimerPlayable() {}

        virtual void OnFinish() {
            GUARD(onFinishFunc);
            (onFinishFunc)(*this);
        }

        virtual void OnReset() {}

        float TimeDeltaFor(TimeSlice time) {
            GUARDR(IsPlaying(), 0)

            auto result = time.delta;
            result *= timeScale;
            return result;
        }

        virtual void Go() {}

        // MARK: - Updatable

        bool IsFinished() const override {
            return runner.IsFinished();
        }

        void OnUpdate(TimeSlice time) override {
            GUARD(IsPlaying());

            GUARD(onUpdateFunc);
            (onUpdateFunc)(*this, TimeSlice(TimeDeltaFor(time)));
        }

        // MARK: - Playable

        void Stop() override {
            runner.runType = RunType::Once;
            runner.SetIsFinished(true);
        }

        void Reset() override {
            Base::Reset();

            runner.Reset();
            OnReset();
        }

        float Duration() const override {
            return duration;
        }

        void SetDuration(float value) override {
            duration = value;
        }
    };
} // namespace PJ
