// #ifndef PJSOMEANIMATEDEFFECT_H
// #define PJSOMEANIMATEDEFFECT_H
//
// #include "SomeEase.h"
// #include "SomeEffect.h"
// #include "Valve.h"
// #include <memory>
//
///*
// RATING: 4 stars
// Could use unit tests
// CODE REVIEW: 12/26
// */
// namespace PJ {
//    // TODO: review all effects
//    /// Uses a valve->to perform animated changes when focus changes
//    /// When this object receives focus, the valve->is opened over N time
//    /// Check the valve->s state to find a normalized value (0-1.0) to use for
//    /// animations
//    class SomeAnimatedEffect : public SomeEffect {
//    public:
//        using Base = SomeEffect;
//
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
