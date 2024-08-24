// #ifndef PJSOMEANIMATEDEFFECT_H
// #define PJSOMEANIMATEDEFFECT_H
//
// #include "SomeEase.h"
// #include "SomeEffect.h"
// #include "Valve.h"
// #include <memory>
//

#include "Animator.h"

namespace PJ {
    /// Animates a value based on a valve that opens or closes
    template <class T>
    class AnimatedEffect : public SomeEffect {
        Animator<T> animator;
        Valve valve;
    };

    template <class T>
    class SomeAnimatePropertyHandler : public WorldComponent<> {
        WP<WorldNode> target;
        T start;
        T end;

        virtual Animator<T> MakeAnimator() = 0;
    };

    class AnimateScaleHandler : public SomeAnimatePropertyHandler {
        WP<WorldNode> target;
        Vector3 start;
        Vector3 end;
    };
} // namespace PJ

///*
// RATING: 4 stars
// Could use unit tests
// CODE REVIEW: 12/26
// */
// namespace PJ {
//    // TODO: review all effects <- GET RID OF THIS, just use an animator
//    /// Uses a valve->to perform animated changes when focus changes
//    /// When this object receives focus, the valve->is opened over N time
//    /// Check the valve->s state to find a normalized value (0-1.0) to use for
//    /// animations
//    class ValveEffect : public SomeEffect {
//    public:
//        using Base = SomeEffect;
//
//         WP<Effect> child; // The effect that this valve changes
//    protected:
//        float turnOnTime;
//        float turnOffTime;
//
//    public:
//        float TurnOnTime() const {
//            return turnOnTime;
//        }
//
//        void TurnOnTime(float value) {
//            turnOnTime = value;
//            valve->turnOnTimer->duration = value;
//        }
//
//        float TurnOffTime() const {
//            return turnOffTime;
//        }
//
//        void SetTurnOffTime(float value) {
//            turnOffTime = value;
//            valve->turnOffTimer->duration = value;
//        }
//
//    protected:
//        /// Used to animate changes in focus over time
//        SP<Valve> valve = MAKE<Valve>();
//
//        SomeAnimatedEffect(float turnOnTime = 0.3f, float turnOffTime = 0.3f) :
//            turnOnTime(turnOnTime),
//            turnOffTime(turnOffTime) {
//            valve->turnOffTimer =
//                MAKE<TransformTimer>(turnOffTime, Runner::RunType::RunOnce,
//                MAKE<OutSquaredEase>());
//            valve->turnOnTimer =
//                MAKE<TransformTimer>(turnOnTime, Runner::RunType::RunOnce,
//                MAKE<OutSquaredEase>());
//        }
//
//        void Awake() override {
//            Base::Awake();
//
//            valve->turnOffTimer->duration = turnOffTime;
//            valve->turnOnTimer->duration = turnOnTime;
//        }
//
//        void Start() override {
//            Base::Start();
//
//            valve->InputBinary(isOn, InputEffect::Immediate);
//            UpdateAnimatableProperties();
//        }
//
//    public:
//        void OnUpdate(TimeSlice time) override {
//            Base::OnUpdate(time);
//
//            valve->OnUpdate(time);
//            UpdateAnimatableProperties();
//        }
//
//    protected:
//        void OnSwitchChange() override {
//            valve->InputBinary(isOn, InputEffect::Timed);
//        }
//
//        virtual void UpdateAnimatableProperties() {}
//    };
//} // namespace PJ
//
// #endif
