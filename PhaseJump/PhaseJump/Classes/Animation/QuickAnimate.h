#pragma once

#include "Animator.h"
#include "InterpolateFunc.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// Convenience tool for quickly building animations
    class QuickAnimate {
    public:
        using This = QuickAnimate;

        /// Func that makes a set binding func
        template <class Type>
        using MakeBindingFunc = std::function<SetBindingFunc<Type>(WorldNode&)>;

        /// Create an animator maker func of this type
        template <class Type>
        static MakeAnimatorFunc<Type, WorldNode&> Maker(
            float duration, EaseFunc easeFunc, MakeBindingFunc<Type> makeBinding,
            AnimationCycleType cycle = AnimationCycleType::Once, EaseFunc reverseEaseFunc = {}
        ) {
            return [=](Type start, Type end, WorldNode& target) {
                InterpolateFunc<Type> interpolateFunc =
                    InterpolateFuncs::MakeEase(InterpolateFuncs::Make(start, end), easeFunc);
                InterpolateFunc<Type> reverseInterpolateFunc = InterpolateFuncs::MakeEase(
                    InterpolateFuncs::Make(end, start), reverseEaseFunc ? reverseEaseFunc : easeFunc
                );

                return MAKE<Animator<Type>>(
                    interpolateFunc, duration, makeBinding(target), cycle, reverseInterpolateFunc
                );
            };
        }

        /// @return Returns a maker for a node position animation
        static MakeAnimatorFunc<Vector3, WorldNode&> PositionMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        ) {
            return Maker<Vector3>(
                duration, easeFunc,
                [](auto& target) { return [&](auto& value) { target.SetLocalPosition(value); }; },
                cycle, reverseEaseFunc
            );
        }

        /// @return Returns a maker for a scale animation
        static MakeAnimatorFunc<Vector3, WorldNode&> ScaleMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        ) {
            return Maker<Vector3>(
                duration, easeFunc,
                [](auto& target) { return [&](auto& value) { target.SetScale(value); }; }, cycle,
                reverseEaseFunc
            );
        }

        /// @return Returns a maker for a uniform scale animation
        static MakeAnimatorFunc<float, WorldNode&> UniformScaleMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        ) {
            return Maker<float>(
                duration, easeFunc,
                [](auto& target) {
                    return [&](auto& value) { target.SetScale({ value, value, value }); };
                },
                cycle, reverseEaseFunc
            );
        }

        /// @return Returns a maker for an opacity animation
        static MakeAnimatorFunc<float, WorldNode&> OpacityMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        ) {
            return Maker<float>(
                duration, easeFunc,
                [](auto& target) { return [&](auto& value) { target.SetOpacity(value); }; }, cycle,
                reverseEaseFunc
            );
        }

        /// @return Returns a maker for a rotate animation
        static MakeAnimatorFunc<float, WorldNode&> RotateMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        ) {
            return Maker<float>(
                duration, easeFunc,
                [](auto& target) {
                    return [&](auto& value) {
                        target.transform.SetRotation(Angle::DegreesAngle(value));
                    };
                },
                cycle, reverseEaseFunc
            );
        }

        // MARK: Interpolate Bindings

        /// @return Returns a uniform scale binding func
        static SetBindingFunc<float>
        MakeUniformScaleBinding(WorldNode& node, float start, float end, EaseFunc easeFunc) {
            auto interpolateFunc =
                InterpolateFuncs::MakeEase(InterpolateFuncs::Make(start, end), easeFunc);
            return InterpolateFuncs::MakeBinding<float>(interpolateFunc, [&](auto& value) {
                node.transform.SetScale({ value, value, value });
            });
        }
    };

    using QA = QuickAnimate;
} // namespace PJ
