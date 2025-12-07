#pragma once

#include "Base.h"
#include "Playable.h"
#include "Runner.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests (via PlayableTimer)
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// A timer-type playable with run-once or run-repeat behavior
    class TimerPlayable : public Playable {
    public:
        using Base = Playable;
        using This = TimerPlayable;
        using RunType = RunType;

        /// Duration of this playable
        float duration = 0;

        /// Modifies time scale for update events
        float timeScale = 1.0f;

        Runner runner;

        TimerPlayable(float duration, RunType runType) :
            runner(runType),
            duration(duration) {
            runner.onFinishFunc = [this]() { updatable.SetIsFinished(true); };
            runner.onResetFunc = [this]() { this->OnReset(); };
        }

        virtual ~TimerPlayable() {}

        virtual void OnReset() {
            updatable.SetIsFinished(false);
        }

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

        FinishType OnUpdate(TimeSlice time) override {
            GUARDR(IsPlaying(), FinishType::Continue);

            return Base::OnUpdate(TimeSlice(TimeDeltaFor(time)));
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
