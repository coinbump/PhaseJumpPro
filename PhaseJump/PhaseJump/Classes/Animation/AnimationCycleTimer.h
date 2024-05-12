#ifndef PJANIMATIONCYCLETIMER_H
#define PJANIMATIONCYCLETIMER_H

#include "AnimationCycleTypes.h"
#include "Model/Time/Timer.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 1/12/24
 */
namespace PJ
{
    /// <summary>
    /// Handles animation cycle logic for an animation
    /// </summary>
    class AnimationCycleTimer : Updatable
    {
    protected:
        Timer timer;

        AnimationCycleType cycleType;
        AnimationCycleState cycleState;

    public:
        AnimationCycleTimer(float duration, AnimationCycleType cycleType) : timer(duration, SomeTimed::RunType::RunOnce), cycleType(cycleType), cycleState(AnimationCycleState::Forward)
        {
        }

        virtual ~AnimationCycleTimer() {};

        float Progress() const
        {
            switch (cycleState)
            {
                case AnimationCycleState::Reverse:
                    return 1.0f - timer.Progress();
                default:
                    return timer.Progress();
            }
        }

        bool IsFinished() const override
        {
            return timer.IsFinished();
        }

        float CycleTime() const
        {
            return timer.duration;
        }

        void SetCycleTime(float value)
        {
            timer.duration = value;
        }

        AnimationCycleState CycleState() const
        {
            return cycleState;
        }

        void SetCycleState(AnimationCycleState value)
        {
            if (cycleState == value) { return; }
            cycleState = value;
        }

        AnimationCycleType CycleType() const
        {
            return cycleType;
        }

        void SetCycleType(AnimationCycleType value)
        {
            if (cycleType == value) { return; }
            cycleType = value;
        }

        void SetProgress(float progress)
        {
            switch (cycleState)
            {
                case AnimationCycleState::Reverse:
                    timer.SetProgress(1.0f - progress);
                    break;
                default:
                    timer.SetProgress(progress);
                    break;
            }
        }

        void OnUpdate(TimeSlice time) override
        {
            if (timer.IsFinished()) { return; }

            timer.OnUpdate(time);

            if (timer.IsFinished())
            {
                switch (cycleType)
                {
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
    };
}

#endif
