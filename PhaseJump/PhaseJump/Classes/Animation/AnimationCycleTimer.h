#ifndef PJANIMATIONCYCLETIMER_H
#define PJANIMATIONCYCLETIMER_H

#include "AnimationCycleTypes.h"
#include "Model/Time/Timer.h"
#include "Playable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: TODO
 */
namespace PJ {
    /// Handles animation cycle logic for an animation
    // TODO: use composition for behavior, not enums
    class AnimationCycleTimer : Playable {
    public:
        using This = AnimationCycleTimer;

    protected:
        Timer timer;

        AnimationCycleType cycleType;
        AnimationCycleState cycleState;

    public:
        AnimationCycleTimer(float duration, AnimationCycleType cycleType) :
            timer(duration, Runner::RunType::Once),
            cycleType(cycleType),
            cycleState(AnimationCycleState::Forward) {}

        virtual ~AnimationCycleTimer() {};

        bool IsFinished() const override {
            return timer.IsFinished();
        }

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

        void SetProgress(float progress) {
            switch (cycleState) {
            case AnimationCycleState::Reverse:
                timer.SetProgress(1.0f - progress);
                break;
            default:
                timer.SetProgress(progress);
                break;
            }
        }

        void OnUpdate(TimeSlice time) override {
            if (timer.IsFinished()) {
                return;
            }

            timer.OnUpdate(time);

            if (timer.IsFinished()) {
                switch (cycleType) {
                case AnimationCycleType::Once:
                    break;
                case AnimationCycleType::Loop:
                    timer.Reset();
                    break;
                case AnimationCycleType::PingPong:
                    cycleState = Flip(cycleState);
                    timer.Reset();
                    break;
                }
            }
        }

        // MARK: - Playable

        bool IsPlaying() const override {
            return timer.IsPlaying();
        }

        float Progress() const override {
            switch (cycleState) {
            case AnimationCycleState::Reverse:
                return 1.0f - timer.Progress();
            default:
                return timer.Progress();
            }
        }

        float Duration() const override {
            return timer.Duration();
        }

        void Play() override {
            timer.Play();
        }

        void Pause() override {
            timer.Pause();
        }

        void Stop() override {
            timer.Stop();
        }

        float PlayTime() const override {
            return timer.PlayTime();
        }

        void SetPlayTime(float time) override {
            timer.SetPlayTime(time);
        }
    };
} // namespace PJ

#endif
