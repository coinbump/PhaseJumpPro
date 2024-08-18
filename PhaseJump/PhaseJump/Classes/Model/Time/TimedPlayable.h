#pragma once

#include "Base.h"
#include "Function.h"
#include "Playable.h"
#include "Runner.h"

/*
 RATING: 5 stars
 Has unit tests (via Timer)
 CODE REVIEW: 7/7/24
 */
namespace PJ {
    /// Any object that relies on timers to perform tasks
    /// Supports an internal time scale for animating an object's time
    class TimedPlayable : public Playable {
    protected:
        /// If true, timer will stop receiving time events
        bool isPaused = false;

    public:
        using This = TimedPlayable;
        using RunType = Runner::RunType;

        /// Used to speed up/slow down timer
        float timeScale = 1.0f;

        float duration = 0;

        Runner runner;

        // TODO: SomeFunction probably isn't needed, because you can wrap the TimedPlayable in an
        // outer object that has the state that the func needs
        /// Allows  functional composition of behavior
        UP<SomeFunction<void(This&, TimeSlice)>> updateFunc;
        UP<SomeFunction<void(This&)>> onFinishFunc;

        TimedPlayable(float duration, RunType runType) :
            runner(runType),
            duration(duration) {
            runner.onFinishFunc = [this](Runner& runner) { this->OnFinish(); };
            runner.onResetFunc = [this](Runner& runner) { this->OnReset(); };
        }

        virtual ~TimedPlayable() {}

        bool IsRunning() const {
            return !isPaused;
        }

        void Stop() override {
            runner.runType = RunType::RunOnce;
            runner.SetIsFinished(true);
        }

        virtual void Reset() {
            runner.Reset();
            OnReset();
        }

        virtual void OnFinish() {
            GUARD(onFinishFunc);
            (*onFinishFunc)(*this);
        }

        virtual void OnReset() {}

        float TimeDeltaFor(TimeSlice time) {
            // Paused timer. No time events
            if (isPaused) {
                return 0;
            }

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
            GUARD(!isPaused);

            GUARD(updateFunc);
            (*updateFunc)(*this, TimeSlice(TimeDeltaFor(time)));
        }

        void SetDuration(float duration) {
            this->duration = duration;
        }

        // MARK: - Playable

        bool IsPlaying() const override {
            return !isPaused;
        }

        void Play() override {
            isPaused = false;
        }

        void Pause() override {
            isPaused = true;
        }

        float Duration() const override {
            return duration;
        }
    };
} // namespace PJ
