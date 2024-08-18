#pragma once

#include "Binding.h"
#include "Interpolator.h"
#include "Timer.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/15/24
 */
namespace PJ {
    /**
     Animates a value from start to end once

     Value is changed by  interpolator, and a binding.
     Interpolator interpolates start and end values
     Binding sets the new value
     Example: animate a character from positions start to end
     */
    template <class T>
    class Animator : public Updatable {
    protected:
        Timer timer;

    public:
        /// Interpolates value from start to end
        InterpolateFunc<T> interpolator;

        /// Value binding to modify value
        SetBindingFunc<T> binding;

        Animator(InterpolateFunc<T> interpolator, float duration, SetBindingFunc<T> binding) :
            timer(duration, Runner::RunType::RunOnce),
            interpolator(interpolator),
            binding(binding) {}

        float Progress() const {
            return timer.Progress();
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            GUARD(!timer.IsFinished())
            timer.OnUpdate(time);

            auto curveValue = interpolator(timer.Progress());
            binding(curveValue);
        }

        bool IsFinished() const override {
            return timer.IsFinished();
        }
    };
} // namespace PJ
