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
        using GoFunc = std::function<void()>;

        GoFunc goFunc;

        SomeDriver(GoFunc goFunc) :
            goFunc(goFunc) {}
    };

    /// Used to drive events. Example: emit something after N seconds
    /// Allows us to compose complex behavior for triggering events
    template <class Core = Void>
    class Driver : public SomeDriver {
    public:
        using This = Driver<Core>;
        using Base = SomeDriver;

        Core core;

        Driver(Core core, GoFunc goFunc) :
            Base(goFunc),
            core(core) {}
    };

    /// Drives an event based on a timer
    class TimerDriver : public Driver<Timer> {
    public:
        using Base = Driver<Timer>;

        TimerDriver(float duration, Runner::RunType runType, GoFunc goFunc) :
            Base(Timer(duration, runType), goFunc) {

            core.onFinishFunc = [this](TimedPlayable& timedPlayable) {
                GUARD(this->goFunc);
                this->goFunc();
            };
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
