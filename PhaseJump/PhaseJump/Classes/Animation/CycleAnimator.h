#pragma once

#include "AnimationCycleTimer.h"
#include "Binding.h"
#include "Interpolator.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/16/24
 */
namespace PJ {
    /// Animates values in a loop/once/ping-ping animation cycle
    template <class T>
    class CycleAnimator : public Updatable {
    public:
        /// Interpolates between start and end values
        InterpolateFunc<T> interpolator;

        /// (Optional) Interpolates values when cycle is in reverse
        InterpolateFunc<T> reverseInterpolator;

        /// Cycle progress for Once, Loop, PingPong
        UP<AnimationCycleTimer> timer;

        /// Value binding to modify value
        SetBindingFunc<T> binding;

        CycleAnimator(
            InterpolateFunc<T> interpolator, UP<AnimationCycleTimer> timer,
            SetBindingFunc<T> binding
        ) :
            interpolator(interpolator),
            timer(std::move(timer)),
            binding(binding) {}

        float Progress() const {
            return timer->Progress();
        }

        bool IsFinished() const override {
            return timer->IsFinished();
        }

        void OnUpdate(TimeSlice time) override {
            GUARD(timer)
            GUARD(interpolator)
            GUARD(!IsFinished())

            timer->OnUpdate(time);

            GUARD(binding)

            float progress = Progress();
            auto curveValue = interpolator(progress);

            switch (timer->CycleState()) {
            case AnimationCycleState::Forward:
                break;
            case AnimationCycleState::Reverse:
                if (reverseInterpolator) {
                    curveValue = reverseInterpolator(1.0f - progress);
                }
                break;
            }

            binding(curveValue);
        }
    };
} // namespace PJ
