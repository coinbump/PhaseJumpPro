#ifndef PJANIMATOR_H
#define PJANIMATOR_H

#include "SomeTimed.h"
#include "Binding.h"
#include "Interpolator.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 2/18/23
 */
namespace PJ
{
    /// <summary>
    /// Animates a value from start to end once once
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

            auto curveValue = interpolator->ValueAt(Progress());
            binding->SetValue(curveValue);
        }
    };
}

#endif
