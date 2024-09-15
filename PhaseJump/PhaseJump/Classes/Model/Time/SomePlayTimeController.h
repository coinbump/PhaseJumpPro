#pragma once

#include "Updatable.h"
#include "Utils.h"
#include <algorithm>

// TODO: Unit tests
namespace PJ {
    /// Time controller for playable animations. Allows time behaviors like
    /// loop, ping-pong, run-once to be composed independently
    class SomePlayTimeController : public Updatable {
    protected:
        Playable& owner;
        float duration;
        float speed;

        float position = 0;

    public:
        SomePlayTimeController(Playable& owner, float duration, float speed = 1.0f) :
            owner(owner),
            duration(duration),
            speed(speed) {}

        float Position() const {
            return position;
        }

        void SetPosition(float value) {
            position = std::clamp(value, 0.0f, duration);
        }

        virtual float Progress() const {
            return position / duration;
        }

        virtual void OnFinishPlay() = 0;

        virtual float TimeDelta(TimeSlice time) const {
            return time.delta * speed;
        }

        virtual bool IsPlayFinished() const {
            return position >= duration;
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            GUARD(time.delta > 0)
            GUARD(owner.IsPlaying())

            position += TimeDelta(time);
            if (IsPlayFinished()) {
                OnFinishPlay();
            }
        }
    };

    /// Plays until we reach the end
    class RunOncePlayTimeController : public SomePlayTimeController {
    protected:
        // MARK: SomePlayTimeController

        void OnFinishPlay() override {
            position = std::clamp(position, 0.0f, duration);
            owner.Pause();
        }

    public:
        using Base = SomePlayTimeController;

        RunOncePlayTimeController(Playable& owner, float duration, float speed = 1.0f) :
            Base(owner, duration, speed) {}
    };

    /// Moves the play position forward, then back
    class PingPongPlayTimeController : public SomePlayTimeController {
    protected:
        bool isForward = true;

    public:
        using Base = SomePlayTimeController;

        PingPongPlayTimeController(Playable& owner, float duration, float speed = 1.0f) :
            Base(owner, duration, speed) {}

        // MARK: SomePlayTimeController

        void OnFinishPlay() override {
            position = std::clamp(position, 0.0f, duration);
            isForward = !isForward;
        }

        bool IsPlayFinished() const override {
            return isForward ? position >= duration : position <= 0;
        }

        float TimeDelta(TimeSlice time) const override {
            float result = Base::TimeDelta(time);
            return isForward ? result : -result;
        }
    };

    /// Loops the play timer when finished
    class LoopPlayTimeController : public SomePlayTimeController {
    protected:
        void OnFinishPlay() override {
            /// Allows for overflow in loop
            position = fmodf(position, duration);
        }

    public:
        using Base = SomePlayTimeController;

        LoopPlayTimeController(Playable& owner, float duration, float speed = 1.0f) :
            Base(owner, duration, speed) {}
    };
} // namespace PJ
