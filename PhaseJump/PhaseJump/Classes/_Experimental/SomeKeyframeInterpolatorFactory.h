// #ifndef PJSOMEKEYFRAMEINTERPOLATORFACTORY_H
// #define PJSOMEKEYFRAMEINTERPOLATORFACTORY_H
//
// #include "Interpolator.h"
// #include <memory>
//
// namespace PJ {
//     // TODO: put this in the timeline track
//
//     template <class T>
//     /// Produces an interpolator based on key values
//     class SomeKeyframeInterpolatorFactory {
//     public:
//         virtual ~SomeKeyframeInterpolatorFactory() {}
//
//         virtual SP<Interpolator<T>>
//         NewInterpolator(T start, T end, SomeKeyframeInterpolatorFactory<T> nextKey) = 0;
//     };
//
//     /// Allows us to set an interpolation curve from one keyframe to the next
//     /// FUTURE: support Unity-style tangent/weight type animation curves
//     class KeyframeInterpolatorFactory : public SomeKeyframeInterpolatorFactory<float> {
//     public:
//         SP<SomeEase> interpolation;
//
//         virtual ~KeyframeInterpolatorFactory() {}
//
//         SP<Interpolator<float>> NewInterpolator(
//             float start, float end, SomeKeyframeInterpolatorFactory<float> nextKey
//         ) override {
//             return MAKE<Interpolator<float>>(
//                 start, end, MAKE<ValueInterpolator<float>>(), interpolation
//             );
//         }
//     };
//
//     // FUTURE: support SplineAnimationCurve
// } // namespace PJ
//
// #endif
