#pragma once

#include "Timer.h"
#include "TransformFunc.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/7/24
 */
namespace PJ {
    /// A timer that runs for N duration, but whose progress is determined by an
    /// transform operation (usually an interpolation curve)
    class TransformTimer : public Timer {
    public:
        // TODO: use SomeTransformer here?
        using Transform = PJ::TransformFunc<float>;
        using Base = Timer;

        Transform transform;

        TransformTimer(float duration, Runner::RunType type, Transform transform) :
            Timer(duration, type),
            transform(transform) {}

        float Progress() const override {
            auto progress = Base::Progress();
            GUARDR(transform, progress)

            return transform(progress);
        }
    };
} // namespace PJ
