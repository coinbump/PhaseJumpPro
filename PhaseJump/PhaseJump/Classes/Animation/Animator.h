#pragma once

#include "AnimationCycleTimer.h"
#include "Binding.h"
#include "InterpolateFunc.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/28/24
 */
namespace PJ {
    /**
     Animates a value from start to end

     Value is changed by  interpolator and a binding.
     Interpolator interpolates start and end values
     Binding sets the new value

     Example: animate a character from positions start to end
     */
    template <class T>
    class Animator : public Updatable {
    protected:
        AnimationCycleTimer timer;

    public:
        using Base = Updatable;
        using This = Animator;

        struct Config {
            float duration = 0.3f;

            /// Interpolates values based on progress
            InterpolateFunc<T> interpolateFunc;

            /// Apples the interpolated value
            SetBindingFunc<T> binding;

            /// Animation cycle type
            AnimationCycleType cycleType = AnimationCycleType::Once;

            /// (Optional) Alternative interpolation when animation is in reverse (Ping-Pong)
            InterpolateFunc<T> reverseInterpolateFunc;
        };

        Animator(Config config) :
            timer(config.duration, config.cycleType) {
            onUpdateFunc = [=](auto& updatable, TimeSlice time) {
                This& animator = *(static_cast<This*>(&updatable));

                animator.timer.OnUpdate(time);

                GUARDR(config.interpolateFunc, FinishType::Finish)

                float progress = animator.timer.Progress();
                auto curveValue = config.interpolateFunc(progress);

                switch (animator.timer.CycleState()) {
                case AnimationCycleState::Forward:
                    break;
                case AnimationCycleState::Reverse:
                    if (config.reverseInterpolateFunc) {
                        curveValue = config.reverseInterpolateFunc(1.0f - progress);
                    }
                    break;
                }

                if (config.binding) {
                    config.binding(curveValue);
                }

                return animator.timer.GetFinishType();
            };
        }

        /// Needed for lambda inside template func that can't construct Config
        Animator(
            InterpolateFunc<T> interpolateFunc, float duration, SetBindingFunc<T> binding,
            AnimationCycleType cycleType = AnimationCycleType::Once,
            InterpolateFunc<T> reverseInterpolateFunc = {}
        ) :
            Animator({ .interpolateFunc = interpolateFunc,
                       .duration = duration,
                       .binding = binding,
                       .cycleType = cycleType,
                       .reverseInterpolateFunc = reverseInterpolateFunc }) {}

        float Progress() const {
            return timer.Progress();
        }

        void SetProgress(float value) {
            timer.SetProgress(value);

            // Sync binding state
            isFinished = onUpdateFunc(*this, { 0 }) == FinishType::Finish;
        }
    };

    template <class Type, class Target>
    using MakeAnimatorFunc =
        std::function<UP<Animator<Type>>(Type start, Type end, Target& target)>;
} // namespace PJ
