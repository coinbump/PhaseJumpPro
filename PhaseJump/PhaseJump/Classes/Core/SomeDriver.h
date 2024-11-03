#pragma once

#include "Timer.h"
#include "Updatable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/31/24
 */
namespace PJ {
    class Void;

    /// Used to drive events. Example: emit something after N seconds
    /// Allows us to compose complex behavior for triggering events
    class SomeDriver : public Updatable {
    public:
        using This = SomeDriver;
        using ActionFunc = std::function<void()>;

        ActionFunc action;

        SomeDriver(ActionFunc action) :
            action(action) {}
    };

    /// Used to drive events. Example: emit something after N seconds
    /// Allows us to compose complex behavior for triggering events
    template <class Core = Void>
    class Driver : public SomeDriver {
    public:
        using This = Driver<Core>;
        using Base = SomeDriver;

        Core core{};

        Driver(Core core, ActionFunc action) :
            Base(action),
            core(core) {}
    };

    /// Drives an event based on a timer
    class TimerDriver : public Driver<Timer> {
    public:
        using Base = Driver<Timer>;

        TimerDriver(float duration, RunType runType, ActionFunc action) :
            Base(Timer(duration, runType), action) {

            core.onFinishFunc = [this](auto& timedPlayable) {
                GUARD(this->action);
                this->action();
            };
        }

        float Duration() const {
            return core.Duration();
        }

        void SetDuration(float value) {
            core.SetDuration(value);
        }

        // MARK: Updatable

        bool IsFinished() const override {
            return core.IsFinished();
        }

        void OnUpdate(TimeSlice time) override {
            core.OnUpdate(time);
        }
    };
} // namespace PJ
