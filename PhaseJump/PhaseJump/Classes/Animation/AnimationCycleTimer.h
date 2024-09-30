#pragma once

#include "AnimationCycleTypes.h"
#include "Playable.h"
#include "Timer.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Handles animation cycle logic for an animation
    /// Alternatively, ping pong animations can be accomplished by turning a valve on and off
    class AnimationCycleTimer : public Playable {
    public:
        using Base = Playable;
        using This = AnimationCycleTimer;
        using OnCycleFunc = std::function<void(This&)>;

    protected:
        Timer timer;
        Playable* playDriver = &timer;

        AnimationCycleType cycleType;
        AnimationCycleState cycleState;

    public:
        /// Called when the timer starts a new animation cycle
        OnCycleFunc onCycleFunc;

        AnimationCycleTimer(float duration, AnimationCycleType cycleType) :
            timer(duration, Runner::RunType::Once),
            cycleType(cycleType),
            cycleState(AnimationCycleState::Forward) {
            timer.SetOnPlayTimeChangeFunc([this](auto& playable) {
                GUARD(onPlayTimeChangeFunc);
                onPlayTimeChangeFunc(*this);
            });
        }

        virtual ~AnimationCycleTimer() {};

        float CycleTime() const {
            return timer.duration;
        }

        void SetCycleTime(float value) {
            timer.duration = value;
        }

        AnimationCycleState CycleState() const {
            return cycleState;
        }

        void SetCycleState(AnimationCycleState value) {
            if (cycleState == value) {
                return;
            }
            cycleState = value;
        }

        AnimationCycleType CycleType() const {
            return cycleType;
        }

        void SetCycleType(AnimationCycleType value) {
            if (cycleType == value) {
                return;
            }
            cycleType = value;
        }

        bool IsReversed() const {
            return cycleState == AnimationCycleState::Reverse;
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            GUARD(!timer.IsFinished())

            timer.OnUpdate(time);

            GUARD(timer.IsFinished())

            switch (cycleType) {
            case AnimationCycleType::Once:
                break;
            case AnimationCycleType::Loop:
                timer.Reset();

                if (onCycleFunc) {
                    onCycleFunc(*this);
                }
                break;
            case AnimationCycleType::PingPong:
                cycleState = Flip(cycleState);
                timer.Reset();

                if (onCycleFunc) {
                    onCycleFunc(*this);
                }
                break;
            }
        }

        // MARK: - Playable

        void Reset() override {
            Base::Reset();
            cycleState = AnimationCycleState::Forward;
        }

        float PlayTime() const override {
            switch (cycleState) {
            case AnimationCycleState::Reverse:
                return Duration() - timer.PlayTime();
                break;
            default:
                return timer.PlayTime();
            }
        }

        void SetPlayTime(float value) override {
            switch (cycleState) {
            case AnimationCycleState::Reverse:
                timer.SetPlayTime(Duration() - value);
                break;
            default:
                timer.SetPlayTime(value);
            }
        }

        float Progress() const override {
            switch (cycleState) {
            case AnimationCycleState::Reverse:
                return 1.0f - timer.Progress();
            default:
                return timer.Progress();
            }
        }

        void SetProgress(float value) override {
            switch (cycleState) {
            case AnimationCycleState::Reverse:
                timer.SetProgress(1.0f - value);
                break;
            default:
                timer.SetProgress(value);
                break;
            }
        }

        Playable* PlayDriver() const override {
            return playDriver;
        }
    };
} // namespace PJ
