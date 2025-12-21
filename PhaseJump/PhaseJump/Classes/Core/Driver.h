#pragma once

#include "PlayableTimer.h"
#include "Updatable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    class Void;

    /// Used to drive events. Example: emit something after N seconds
    /// Allows us to compose complex behavior for triggering events
    class Driver : public SomeUpdatable {
    public:
        using This = Driver;
        using ActionFunc = std::function<void()>;

        ActionFunc actionFunc;

        Driver(ActionFunc action) :
            actionFunc(action) {}

        virtual void Run() {
            GUARD(actionFunc)
            actionFunc();
        }
    };

    /// Used to drive events. Example: emit something after N seconds
    /// Allows us to compose complex behavior for triggering events
    template <class Core = Void>
    class CoreDriver : public Driver {
    public:
        using This = CoreDriver<Core>;
        using Base = Driver;

        Core core{};

        CoreDriver(Core const& core, ActionFunc actionFunc) :
            Base(actionFunc),
            core(core) {}
    };

    /// Drives an event based on a timer
    class TimerDriver : public CoreDriver<PlayableTimer> {
    public:
        using Base = CoreDriver<PlayableTimer>;

        TimerDriver(float duration, RunType runType, ActionFunc _action) :
            Base(PlayableTimer(duration, runType), _action) {

            core.updatable.onFinishFunc = [this](auto& timedPlayable) { Run(); };
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
