#pragma once

#include "Animator.h"
#include "InterpolateFunc.h"
#include "Utils.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// Convenience tool for quickly building animations.
    namespace AnimateFuncs {
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
                    InterpolateFuncs::Ease(InterpolateFuncs::Make(start, end), easeFunc);
                InterpolateFunc<Type> reverseInterpolateFunc = InterpolateFuncs::Ease(
                    InterpolateFuncs::Make(end, start), reverseEaseFunc ? reverseEaseFunc : easeFunc
                );

                return NEW<Animator<Type>>(
                    interpolateFunc, duration, makeBinding(target), cycle, reverseInterpolateFunc
                );
            };
        }

        /// @return Returns a maker for a node position animation
        MakeAnimatorFunc<Vector3, WorldNode&> PositionMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        );

        /// @return Returns a maker for a scale animation
        MakeAnimatorFunc<Vector3, WorldNode&> ScaleMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        );

        /// @return Returns a maker for a uniform scale animation
        MakeAnimatorFunc<float, WorldNode&> UniformScaleMaker2D(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        );

        /// @return Returns a maker for an opacity animation
        MakeAnimatorFunc<float, WorldNode&> OpacityMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        );

        /// @return Returns a maker for a rotate animation
        MakeAnimatorFunc<float, WorldNode&> RotateMaker(
            float duration, EaseFunc easeFunc, AnimationCycleType cycle = AnimationCycleType::Once,
            EaseFunc reverseEaseFunc = {}
        );

        // MARK: Interpolate Bindings

        /// @return Returns a uniform scale binding func
        SetBindingFunc<float>
        MakeUniformScaleBinding2D(WorldNode& node, float start, float end, EaseFunc easeFunc);
    } // namespace AnimateFuncs
} // namespace PJ
