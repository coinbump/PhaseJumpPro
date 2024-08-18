// #pragma once
//
// #include "Playable.h"
// #include "SomeKeyframe.h"
// #include "Updatable.h"
// #include "VectorList.h"
// #include <stdio.h>
//
// namespace PJ {
//     class Playable;
//
//     struct TimelineValue {
//         UP<SomeKeyframe> keyframe;
//
//         TimelineValue(UP<SomeKeyframe>& keyframe) :
//             keyframe(std::move(keyframe)) {}
//
//         // FUTURE: support metadata like notes, curves, modifiers as needed
//     };
//
//     /// A timeline has a start time, duration, and keyframes
//     class Timeline : public Updatable {
//     public:
//         using Value = TimelineValue;
//         using MathType = float;
//         using Keyframe = SomeKeyframe;
//         using KeyframeList = VectorList<UP<Keyframe>>;
//         using ValueList = VectorList<UP<Value>>;
//
//     protected:
//         ValueList values;
//
//         // Drives timeline play state
//         SP<Playable> playable;
//
//     public:
//         ValueList& Values() {
//             return values;
//         }
//
//         virtual ~Timeline() {}
//
//         void Add(UP<Keyframe>& _keyframe, MathType time) {
//             if (!_keyframe) {
//                 return;
//             }
//             _keyframe->time = time;
//
//             // FUTURE: subdivide/parallel keyframe search to optimize
//             for (int i = 0; i < values.Count(); i++) {
//                 auto& value = values[i];
//                 if (value->keyframe->time == _keyframe->time) {
//                     value->keyframe = std::move(_keyframe);
//                     return;
//                 }
//
//                 if (value->keyframe->time > _keyframe->time) {
//                     values.insert(values.begin() + i, std::make_unique<Value>(_keyframe));
//                     return;
//                 }
//             }
//
//             values.push_back(std::make_unique<Value>(_keyframe));
//         }
//
//         void RemoveAt(MathType time) {
//             // FUTURE: subdivide/parallel keyframe search to optimize
//             for (auto& i = values.begin(); i != values.end(); i++) {
//                 auto& value = *i;
//                 if (value->keyframe && value->keyframe->time == time) {
//                     values.erase(i);
//                     return;
//                 }
//             }
//         }
//
//         Keyframe* KeyframeBefore(MathType time) {
//             for (auto& i = values.rbegin(); i != values.rend(); i++) {
//                 auto& value = *i;
//                 if (value->keyframe && value->keyframe->time < time) {
//                     return value->keyframe.get();
//                 }
//             }
//
//             return nullptr;
//         }
//
//         Keyframe* KeyframeAfterOrAt(MathType time) {
//             for (auto& value : values) {
//                 if (value->keyframe->time >= time) {
//                     return value->keyframe.get();
//                 }
//             }
//
//             return nullptr;
//         }
//
//         void OnUpdate(TimeSlice time) override {
//             auto playable = this->playable;
//             if (nullptr == playable) {
//                 return;
//             }
//             playable->OnUpdate(time);
//         }
//
//         bool IsFinished() const override {
//             return false;
//         }
//     };
//
//     template <class T>
//     class ValueTimeline : public Timeline {
//     public:
//         /// Produces the interpolator for the keyframes
//         SP<SomeKeyframeInterpolatorFactory<T>> interpolatorFactory;
//     };
// } // namespace PJ
