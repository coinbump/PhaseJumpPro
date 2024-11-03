#pragma once

#include "AnimationCycleTimer.h"
#include "Binding.h"
#include "InterpolateFunc.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/21/24
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
        AnimationCycleTimer timer;

    public:
        using Base = Updatable;
        using This = Animator;

        Animator(
            InterpolateFunc<T> interpolateFunc, float duration, SetBindingFunc<T> binding,
            AnimationCycleType cycleType = AnimationCycleType::Once,
            InterpolateFunc<T> reverseInterpolateFunc = {}
        ) :
            timer(duration, cycleType) {
            onUpdateFunc = [=](auto& updatable, TimeSlice time) {
                This& animator = *(static_cast<This*>(&updatable));

                animator.timer.OnUpdate(time);

                GUARDR(interpolateFunc, FinishType::Finish)

                float progress = animator.timer.Progress();
                auto curveValue = interpolateFunc(progress);

                switch (animator.timer.CycleState()) {
                case AnimationCycleState::Forward:
                    break;
                case AnimationCycleState::Reverse:
                    if (reverseInterpolateFunc) {
                        curveValue = reverseInterpolateFunc(1.0f - progress);
                    }
                    break;
                }

                if (binding) {
                    binding(curveValue);
                }

                return animator.timer.GetFinishType();
            };
        }

        float Progress() const {
            return timer.Progress();
        }

        void SetProgress(float value) {
            timer.SetProgress(value);

            // Sync binding state
            isFinished = onUpdateFunc(*this, { 0 }) == FinishType::Finish;
        }
    };

    // Animators are SP for storing in Updatables
    template <class Type, class Obj>
    using MakeAnimatorFunc = std::function<SP<Animator<Type>>(Type start, Type end, Obj& target)>;
} // namespace PJ
