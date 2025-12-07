#pragma once

#include "PlayableTimer.h"
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
    class SomeDriver : public SomeUpdatable {
    public:
        using This = SomeDriver;
        using ActionFunc = std::function<void()>;

        ActionFunc action;
        Updatable updatable;

        SomeDriver(ActionFunc action) :
            action(action) {}

        // MARK: SomeUpdatable

        FinishType OnUpdate(TimeSlice time) override {
            return updatable.OnUpdate(time);
        }

        bool IsFinished() const override {
            return updatable.IsFinished();
        }
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
    class TimerDriver : public Driver<PlayableTimer> {
    public:
        using Base = Driver<PlayableTimer>;

        TimerDriver(float duration, RunType runType, ActionFunc action) :
            Base(PlayableTimer(duration, runType), action) {

            core.updatable.onFinishFunc = [this](auto& timedPlayable) {
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

        FinishType OnUpdate(TimeSlice time) override {
            return core.OnUpdate(time);
        }
    };
} // namespace PJ
