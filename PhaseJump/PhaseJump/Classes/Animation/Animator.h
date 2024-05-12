#ifndef PJANIMATOR_H
#define PJANIMATOR_H

#include "SomeTimed.h"
#include "Binding.h"
#include "Interpolator.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 1/12/24
 */
namespace PJ
{
    /// <summary>
    /// Animates a value from start to end once
    /// Value is changed by  interpolator (defines how to interpolate the value), and a binding (updates the value)
    /// Example: animate a character from positions start to end
    /// </summary>
    template <class T>
    class Animator : public SomeTimed
    {
    protected:
        /// <summary>
        /// Time value of animator
        /// </summary>
        float time = 0;

    public:
        using Base = SomeTimed;

        /// <summary>
        /// Interpolates from start to end
        /// </summary>
        SP<Interpolator<T>> interpolator;

        /// <summary>
        /// Value binding to modify value
        /// </summary>
        SP<SetBinding<T>> binding;

        Animator(SP<Interpolator<T>> interpolator, float duration, SP<SetBinding<T>> binding) :
            Base(duration, SomeRunner::RunType::RunOnce),
            interpolator(interpolator),
            binding(binding)
        {
        }

        float Progress() const override { return std::max(0.0f, std::min(1.0f, time / duration)); }

        void OnUpdate(TimeSlice time) override
        {
            if (IsFinished()) { return; }

            this->time += time.delta;
            SetIsFinished(this->time >= duration);

            if (nullptr == interpolator) { return; }
            auto curveValue = interpolator->ValueAt(Progress());

            if (nullptr == binding) { return; }
            binding->SetValue(curveValue);
        }
    };
}

#endif
