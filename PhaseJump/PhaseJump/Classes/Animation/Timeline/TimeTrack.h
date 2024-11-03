#pragma once

#include "AnimationCycleTimer.h"
#include "Binding.h"
#include "DiagnoseModel.h"
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

    /// Model for building a keyframe
    template <class Type>
    struct KeyframeModel {
        Type value{};

        /// Fallback if optionals are not defined. The absolute position of the keyframe
        float time{};

        /// If specified, the relative postion of the new keyframe after the final keyframe
        std::optional<float> deltaTime;
    };

    /// Time track protocl
    class SomeTimeTrack : public Playable {
    public:
        using Base = Playable;
        using This = SomeTimeTrack;

        String id;

        SomeTimeTrack(String id) :
            id(id) {}
    };

    /// Contains keyframes that determine values, and a binding func that binds those values
    /// to an animation
    template <class Type>
    class TimeTrack : public SomeTimeTrack {
    public:
        using Base = SomeTimeTrack;
        using This = TimeTrack;
        using MathType = float;
        using Keyframe = PJ::ValueKeyframe<Type>;
        using KeyframeList = VectorList<UP<Keyframe>>;

        using MakeKeyframeFunc = std::function<UP<Keyframe>(This&)>;
        using MakeEaseFuncFunc = std::function<EaseFunc(This&, Keyframe& start, Keyframe& end)>;

    protected:
        /// Keyframes are positioned along the timeline at time positions
        KeyframeList keyframes;

        AnimationCycleTimer timer;
        Playable* controller = &timer;

    public:
        /// Used to enable extra logging for this object
        // TODO: TEMP CODE, DO NOT KEEP
        DiagnoseModel _diagnose{ true };

        /// Applies the timeline's current value via binding
        /// Example: set uniform scale based on timeline float value
        SetBindingFunc<Type> setBindingFunc;

        /// Makes an ease func based on the current keyframe values
        MakeEaseFuncFunc makeEaseFuncFunc;

        MakeKeyframeFunc makeKeyframeFunc = [](auto& track) {
            UP<Keyframe> result = NEW<Keyframe>();
            return result;
        };

        struct Config {
            String id;
            float duration = 1.0f;
            AnimationCycleType cycleType = AnimationCycleType::Once;
            KeyedTimeType keyedTimeType = KeyedTimeType::Interpolate;
        };

        TimeTrack(Config const& config) :
            Base(config.id),
            timer(config.duration, config.cycleType) {
            timer.SetOnPlayTimeChangeFunc([this](auto& playable) { OnPlayTimeChange(); });

            switch (config.keyedTimeType) {
            case KeyedTimeType::Interpolate:
                makeEaseFuncFunc = [](This& timeline, Keyframe& start, Keyframe& end) {
                    if (start.outEaseFunc && !end.inEaseFunc) {
                        return start.outEaseFunc;
                    } else if (!start.outEaseFunc && end.inEaseFunc) {
                        return end.inEaseFunc;
                    } else if (start.outEaseFunc && end.inEaseFunc) {
                        // FUTURE: should we combine both ease funcs together?
                        return end.inEaseFunc;
                    }

                    return EaseFuncs::linear;
                };
                break;
            case KeyedTimeType::Discrete:
                break;
            }
        }

        virtual ~TimeTrack() {}

        KeyframeList const& Keyframes() const {
            return keyframes;
        }

        bool IsReversed() const {
            return timer.IsReversed();
        }

        Keyframe* Add(UP<Keyframe>& kf, MathType time) {
            kf->time = time;

            for (int i = 0; i < keyframes.size(); i++) {
                auto& value = keyframes[i];
                if (value->time == kf->time) {
                    value = std::move(kf);
                    return value.get();
                }

                if (value->time > kf->time) {
                    keyframes.insert(keyframes.begin() + i, std::move(kf));
                    return keyframes[i].get();
                }
            }

            keyframes.push_back(std::move(kf));
            return keyframes[keyframes.size() - 1].get();
        }

        /// Adds a keyframe based on the keyframe model
        /// @returns Returns the new keyframe
        Keyframe* Add(KeyframeModel<Type> const& model) {
            UP<Keyframe> kf = makeKeyframeFunc(*this);
            kf->value = model.value;

            float time = model.time;
            if (model.deltaTime) {
                time = *model.deltaTime;
                if (!IsEmpty(keyframes)) {
                    auto& lastKeyframe = keyframes[keyframes.size() - 1];
                    time += lastKeyframe->time;
                }
            }

            return Add(kf, time);
        }

        /// Adds a keyframe at the specified time
        /// @returns Returns the new keyframe
        Keyframe* AddAt(MathType time) {
            UP<Keyframe> kf = makeKeyframeFunc(*this);
            return Add(kf, time);
        }

        /// Removes any keyframe at the specified time
        void RemoveAt(MathType time) {
            auto i = FindAt(time);
            GUARD(i != keyframes.end());
            keyframes.erase(i);
        }

        /// @return Returns an iterator for a keyframe at the specified time
        auto FindAt(MathType time) const {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.begin(); i < keyframes.end(); i++) {
                auto& value = *i;
                if (value->time == time) {
                    return i;
                }
            }

            return keyframes.end();
        };

        /// @return Returns an iterator for a keyframe before the specified time
        auto FindBefore(MathType time) const {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.rbegin(); i != keyframes.rend(); i++) {
                auto& value = *i;
                if (value->time < time) {
                    return i;
                }
            }

            return keyframes.rend();
        }

        /// @return Returns an iterator for a keyframe at or before the specified time
        auto FindAtOrBefore(MathType time) const {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.rbegin(); i != keyframes.rend(); i++) {
                auto& value = *i;
                if (value->time <= time) {
                    return i;
                }
            }

            return keyframes.rend();
        }

        /// @return Returns an iterator for a keyframe at or after the specified time
        auto FindAtOrAfter(MathType time) const {
            // FUTURE: subdivide search to optimize if needed
            for (auto i = keyframes.begin(); i < keyframes.end(); i++) {
                auto& value = *i;
                if (value->time >= time) {
                    return i;
                }
            }

            return keyframes.end();
        }

        Keyframe* KeyframeAt(MathType time) const {
            auto i = FindAt(time);
            GUARDR(i != keyframes.end(), nullptr);
            return (*i).get();
        }

        /// @return Returns the first keyframe at or after the specified time
        Keyframe* KeyframeAtOrAfter(MathType time) const {
            auto i = FindAtOrAfter(time);
            GUARDR(i != keyframes.end(), nullptr);
            return (*i).get();
        }

        Keyframe* KeyframeBefore(MathType time) const {
            auto i = FindBefore(time);
            GUARDR(i != keyframes.rend(), nullptr);
            return (*i).get();
        }

        Keyframe* KeyframeAtOrBefore(MathType time) const {
            auto i = FindAtOrBefore(time);
            GUARDR(i != keyframes.rend(), nullptr);
            return (*i).get();
        }

        Type ValueAt(MathType time) const {
            auto keyframe = KeyframeAt(time);
            GUARDR(keyframe, {})

            return keyframe->Value();
        }

        Type Value() const {
            return ValueAt(PlayTime());
        }

        AnimationCycleType CycleType() const {
            return timer.CycleType();
        }

        void SetCycleType(AnimationCycleType cycleType) {
            timer.SetCycleType(cycleType);
        }

        // MARK: Playable

        void OnPlayTimeChange() override {
            Base::OnPlayTimeChange();

            PJ::LogIf(_diagnose, "Log: SomeTimeTrack: Play time: ", MakeString(PlayTime()));

            GUARD(setBindingFunc)
            float position = Controller()->PlayTime();
            PJ::LogIf(
                _diagnose, "Log: SomeTimeTrack: Controller Play time: ", MakeString(position)
            );

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
                    PJ::LogIf(_diagnose, "Log: SomeTimeTrack: No Keyframes");
                    // No keyframes, nothing to animate
                    return;
                }

                auto interpolatorFunc = InterpolateFuncs::MakeEase(
                    InterpolateFuncs::Make(startValue, endValue), easeFunc
                );
                Type value = interpolatorFunc(progress);

                PJ::LogIf(
                    _diagnose, "Log: SomeTimeTrack: Set Binding Interpolate: ", MakeString(value)
                );
                setBindingFunc(value);
            } else {
                auto kfStart = KeyframeAtOrBefore(position);

                // Discrete keyframes use the value before the current time position
                if (kfStart) {
                    PJ::LogIf(
                        _diagnose,
                        "Log: SomeTimeTrack: Set Binding Discrete: ", MakeString(kfStart->value)
                    );
                    setBindingFunc(kfStart->value);
                }
            }
        }

        Playable* Controller() const override {
            return controller;
        }
    };
} // namespace PJ
