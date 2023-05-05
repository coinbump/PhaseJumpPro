#ifndef PJCYCLEANIMATOR_H
#define PJCYCLEANIMATOR_H

#include "Interpolator.h"
#include "AnimationCycleTimer.h"
#include "Binding.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/17/22
 */
namespace PJ
{
    /// <summary>
    /// Animates values in a loop/once/ping-ping animation cycle
    /// </summary>
    template <class T>
    class CycleAnimator : public Updatable
    {
    public:
        enum class ReverseType {
            /// <summary>
            /// On reverse direction, apply the interpolation of the animation curve in reverse
            /// </summary>
            Rewind,

            /// <summary>
            /// On reverse direction, match the interpolation of the animation curve used in the forward direction
            /// </summary>
            Match
        };

        /// <summary>
        /// Interpolation, start, and end values
        /// </summary>
        SP<Interpolator<T>> interpolator;

        /// <summary>
        /// Cycle progress for Once, Loop, PingPong
        /// </summary>
        SP<AnimationCycleTimer> timer;

        /// <summary>
        /// Value binding to modify value
        /// </summary>
        SP<SetBinding<T>> binding;

        ReverseType reverseType = ReverseType::Match;

        CycleAnimator(SP<Interpolator<T>> interpolator,
                      SP<AnimationCycleTimer> timer,
                      SP<SetBinding<T>> binding)
        : interpolator(interpolator),
        timer(timer),
        binding(binding)
        {
        }

        float Progress() const {
            return timer->Progress();
        }

        bool IsFinished() const override {
            return timer->IsFinished();
        }

        void OnUpdate(TimeSlice time) override
        {
            if (IsFinished()) { return; }

            timer->OnUpdate(time);

            float progress = Progress();
            auto curveValue = interpolator->ValueAt(progress);

            switch (timer->CycleState()) {
                case AnimationCycleState::Forward:
                    break;
                case AnimationCycleState::Reverse:
                    switch (reverseType)
                    {
                        case ReverseType::Rewind:
                            break;
                        case ReverseType::Match:
                            auto reverseCurve = MAKE<Interpolator<T>>(interpolator->end, interpolator->start, interpolator->valueInterpolator, interpolator->transform);
                            curveValue = reverseCurve->ValueAt(1.0f - progress);
                            break;
                    }
                    break;
            }

            binding->SetValue(curveValue);
        }
    };
}

#endif
