#pragma once

#include "AnimationCycleTimer.h"
#include "Binding.h"
#include "InterpolateFunc.h"
#include "Keyframe.h"
#include "Updatable.h"
#include "VectorList.h"
#include <stdio.h>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/26/24
 */
namespace PJ {
    class Playable;

    /// Defines the default behavior of the time track
    enum TimeTrackValueType {
        /// Key frame values are interpolated
        Interpolate,

        /// Each key frame value is a distinct value
        /// Example: frame id in an animation
        Discrete
    };

    /// Contains keyframes that determine values, and a binding func that binds those values
    /// to an animation
    template <class Type, class Keyframe = PJ::Keyframe<Type>>
    class TimeTrack : public Playable {
    public:
        using Base = Playable;
        using This = TimeTrack;
        using MathType = float;
        using ValueType = TimeTrackValueType;

        using KeyframeList = VectorList<Keyframe>;
        using MakeEaseFuncFunc = std::function<EaseFunc(This&, Keyframe& start, Keyframe& end)>;

    protected:
        /// Keyframes are positioned along the timeline at time positions
        KeyframeList keyframes;

        AnimationCycleTimer timer;
        Playable* playDriver = &timer;

    public:
        /// Applies the timeline's current value via binding
        /// Example: set uniform scale based on timeline float value
        SetBindingFunc<Type> setBindingFunc;

        /// Makes an ease func based on the current keyframe values
        MakeEaseFuncFunc makeEaseFuncFunc;

        TimeTrack(
            float duration, AnimationCycleType cycleType = AnimationCycleType::Once,
            ValueType valueType = ValueType::Interpolate
        ) :
            timer(duration, cycleType) {
            timer.SetOnPlayTimeChangeFunc([this](auto& playable) { OnPlayTimeChange(); });

            switch (valueType) {
            case ValueType::Interpolate:
                makeEaseFuncFunc = [](This& timeline, Keyframe& start, Keyframe& end) {
                    auto startEaseKF = dynamic_cast<EaseKeyframe<Type>*>(&start);
                    auto endEaseKF = dynamic_cast<EaseKeyframe<Type>*>(&end);
                    GUARDR(startEaseKF && endEaseKF, EaseFuncs::linear)

                    if (startEaseKF->core.outEaseFunc && !endEaseKF->core.inEaseFunc) {
                        return startEaseKF->core.outEaseFunc;
                    } else if (!startEaseKF->core.outEaseFunc && endEaseKF->core.inEaseFunc) {
                        return endEaseKF->core.inEaseFunc;
                    } else if (startEaseKF->core.outEaseFunc && endEaseKF->core.inEaseFunc) {
                        return endEaseKF->core.inEaseFunc;
                    }

                    return EaseFuncs::linear;
                };
                break;
            case ValueType::Discrete:
                break;
            }
        }

        virtual ~TimeTrack() {}

        KeyframeList const& Keyframes() const {
            return keyframes;
        }

        KeyframeList::iterator Add(Keyframe kf, MathType time) {
            kf.time = time;

            for (int i = 0; i < keyframes.size(); i++) {
                auto& value = keyframes[i];
                if (value.time == kf.time) {
                    value = kf;
                    return keyframes.begin() + i;
                }

                if (value.time > kf.time) {
                    keyframes.insert(keyframes.begin() + i, kf);
                    return keyframes.begin() + i;
                }
            }

            keyframes.push_back(kf);
            return keyframes.begin() + (keyframes.size() - 1);
        }

        KeyframeList::iterator AddAt(MathType time) {
            return Add({}, time);
        }

        void RemoveAt(MathType time) {
            auto i = FindAt(time);
            GUARD(i != keyframes.end());
            keyframes.erase(i);
        }

        KeyframeList::iterator FindAt(MathType time) {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.begin(); i < keyframes.end(); i++) {
                auto& value = *i;
                if (value.time == time) {
                    return i;
                }
            }

            return keyframes.end();
        };

        KeyframeList::reverse_iterator FindBefore(MathType time) {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.rbegin(); i != keyframes.rend(); i++) {
                auto& value = *i;
                if (value.time < time) {
                    return i;
                }
            }

            return keyframes.rend();
        }

        KeyframeList::reverse_iterator FindAtOrBefore(MathType time) {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.rbegin(); i != keyframes.rend(); i++) {
                auto& value = *i;
                if (value.time <= time) {
                    return i;
                }
            }

            return keyframes.rend();
        }

        KeyframeList::iterator FindAtOrAfter(MathType time) {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.begin(); i < keyframes.end(); i++) {
                auto& value = *i;
                if (value.time >= time) {
                    return i;
                }
            }

            return keyframes.end();
        }

        Keyframe* KeyframeAt(MathType time) {
            auto i = FindAt(time);
            GUARDR(i != keyframes.end(), nullptr);
            return &(*i);
        }

        Keyframe* KeyframeAtOrAfter(MathType time) {
            auto i = FindAtOrAfter(time);
            GUARDR(i != keyframes.end(), nullptr);
            return &(*i);
        }

        Keyframe* KeyframeBefore(MathType time) {
            auto i = FindBefore(time);
            GUARDR(i != keyframes.rend(), nullptr);
            return &(*i);
        }

        Keyframe* KeyframeAtOrBefore(MathType time) {
            auto i = FindAtOrBefore(time);
            GUARDR(i != keyframes.rend(), nullptr);
            return &(*i);
        }

        // MARK: Playable

        void OnPlayTimeChange() override {
            Base::OnPlayTimeChange();

            GUARD(setBindingFunc)
            float position = PlayDriver()->PlayTime();

            if (makeEaseFuncFunc) {
                Type startValue{};
                Type endValue{};
                float progress{};

                auto kfStart = KeyframeBefore(position);
                auto kfEnd = KeyframeAtOrAfter(position);

                // Interpolated keyframes use an ease func to set the interpolated value
                auto easeFunc = EaseFuncs::linear;

                if (kfEnd && nullptr == kfStart) {
                    // There is an end value but no start value, so use the end value
                    startValue = kfEnd->value;
                    endValue = kfEnd->value;
                } else if (kfStart && nullptr == kfEnd) {
                    // There is a start value but no end value, so use the start value
                    startValue = kfStart->value;
                    endValue = kfStart->value;
                } else if (kfEnd && kfStart) {
                    startValue = kfStart->value;
                    endValue = kfEnd->value;

                    float keyframeDistance = kfEnd->time - kfStart->time;
                    progress = (position - kfStart->time) / keyframeDistance;

                    easeFunc = makeEaseFuncFunc(*this, *kfStart, *kfEnd);
                } else {
                    // No keyframes, nothing to animate
                    return;
                }

                auto interpolatorFunc = InterpolateFuncs::MakeEase(
                    InterpolateFuncs::Make(startValue, endValue), easeFunc
                );
                Type value = interpolatorFunc(progress);

                setBindingFunc(value);
            } else {
                auto kfStart = KeyframeAtOrBefore(position);

                // Discrete keyframes use the value before the current time position
                if (kfStart) {
                    setBindingFunc(kfStart->value);
                }
            }
        }

        Playable* PlayDriver() const override {
            return playDriver;
        }
    };
} // namespace PJ
