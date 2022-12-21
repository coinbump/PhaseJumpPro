#ifndef PJTRANSFORMTIMER_H
#define PJTRANSFORMTIMER_H

#include "SomeTransform.h"
#include "Timer.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 11/27/22
 */
namespace PJ
{
    /// <summary>
    /// A timer that runs for N duration, but whose progress is determined by an
    /// transform operation (usually an interpolation curve)
    /// </summary>
    class TransformTimer : public Timer
    {
    public:
        using Base = Timer;

        std::shared_ptr<SomeValueTransform<float>> transform;

        TransformTimer(float duration, SomeTimed::RunType type, std::shared_ptr<SomeValueTransform<float>> transform)
            : Timer(duration, type), transform(transform)
        {
        }

        float Progress() const override
        {
            auto progress = Base::Progress();
            if (!transform)
            {
                return progress;
            }

            return transform->Transform(progress);
        }
    };
}

#endif
