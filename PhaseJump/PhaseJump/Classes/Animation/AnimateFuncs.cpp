#include "AnimateFuncs.h"

using namespace std;
using namespace PJ;

MakeAnimatorFunc<Vector3, WorldNode&> AnimateFuncs::PositionMaker(
    float duration, EaseFunc easeFunc, AnimationCycleType cycle, EaseFunc reverseEaseFunc
) {
    return Maker<Vector3>(
        duration, easeFunc,
        [](auto& target) { return [&](auto& value) { target.SetLocalPosition(value); }; }, cycle,
        reverseEaseFunc
    );
}

/// @return Returns a maker for a scale animation
MakeAnimatorFunc<Vector3, WorldNode&> AnimateFuncs::ScaleMaker(
    float duration, EaseFunc easeFunc, AnimationCycleType cycle, EaseFunc reverseEaseFunc
) {
    return Maker<Vector3>(
        duration, easeFunc,
        [](auto& target) { return [&](auto& value) { target.SetScale(value); }; }, cycle,
        reverseEaseFunc
    );
}

/// @return Returns a maker for a uniform scale animation
MakeAnimatorFunc<float, WorldNode&> AnimateFuncs::UniformScaleMaker2D(
    float duration, EaseFunc easeFunc, AnimationCycleType cycle, EaseFunc reverseEaseFunc
) {
    return Maker<float>(
        duration, easeFunc,
        [](auto& target) { return [&](auto& value) { target.SetScale({ value, value, 1 }); }; },
        cycle, reverseEaseFunc
    );
}

/// @return Returns a maker for an opacity animation
MakeAnimatorFunc<float, WorldNode&> AnimateFuncs::OpacityMaker(
    float duration, EaseFunc easeFunc, AnimationCycleType cycle, EaseFunc reverseEaseFunc
) {
    return Maker<float>(
        duration, easeFunc,
        [](auto& target) { return [&](auto& value) { target.SetOpacity(value); }; }, cycle,
        reverseEaseFunc
    );
}

/// @return Returns a maker for a rotate animation
MakeAnimatorFunc<float, WorldNode&> AnimateFuncs::RotateMaker(
    float duration, EaseFunc easeFunc, AnimationCycleType cycle, EaseFunc reverseEaseFunc
) {
    return Maker<float>(
        duration, easeFunc,
        [](auto& target) {
            return [&](auto& value) { target.transform.SetRotation(Angle::WithDegrees(value)); };
        },
        cycle, reverseEaseFunc
    );
}

// MARK: Interpolate Bindings

/// @return Returns a uniform scale binding func
SetBindingFunc<float> AnimateFuncs::MakeUniformScaleBinding2D(
    WorldNode& node, float start, float end, EaseFunc easeFunc
) {
    auto interpolateFunc = InterpolateFuncs::Ease(InterpolateFuncs::Make(start, end), easeFunc);
    return InterpolateFuncs::Binding<float>(interpolateFunc, [&](auto& value) {
        node.transform.SetScale({ value, value, 1 });
    });
}
