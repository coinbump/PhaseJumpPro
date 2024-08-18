// #ifndef PJVALVE_H
// #define PJVALVE_H
//
// #include "ComposeTimedStateMachine.h"
// #include "SomeEase.h"
// #include "TransformTimer.h"
// #include <memory>
// #include <optional>
//
///*
// RATING: 5 stars
// Has unit tests
// CODE REVIEW: 7/6/23
// */
// namespace PJ {
//    enum class InputEffect {
//        // Change occurs without timer
//        Immediate,
//
//        // Change uses timer
//        Timed
//    };
//
//    enum class ValveStateType {
//        Off,
//        TurnOn, // Valve is turning on (Example: wheel turing to open door)
//        On,     // Valve is on (Example: light switch is on)
//        TurnOff
//    };
//
//    /// Common design pattern in games. Something has 2 states, and it
//    /// transitions from state A to state B over N time Example: a light that
//    /// turns on and off, or a wheel that turns left and right
//    class Valve : public ComposeTimedStateMachine<ValveStateType> {
//    private:
//        std::optional<float> onStateDuration = std::nullopt;  // If not null, hold on state for N
//                                                              // seconds, then turn off
//        std::optional<float> offStateDuration = std::nullopt; // If not null, hold off state for N
//                                                              // seconds, then turn off
//
//    public:
//        using Base = ComposeTimedStateMachine<ValveStateType>;
//        using StateType = ValveStateType;
//
//        std::optional<float> OnStateDuration() const {
//            return onStateDuration;
//        }
//
//        void SetOnStateDuration(std::optional<float> value) {
//            onStateDuration = value;
//
//            if (State() == StateType::On) {
//                if (!onStateDuration) {
//                    sm->SetStateDuration(0);
//                } else {
//                    sm->SetStateDuration(onStateDuration.value());
//                }
//            }
//        }
//
//        std::optional<float> OffStateDuration() const {
//            return offStateDuration;
//        }
//
//        void SetOffStateDuration(std::optional<float> value) {
//            offStateDuration = value;
//
//            if (State() == StateType::Off) {
//                if (!offStateDuration) {
//                    sm->SetStateDuration(0);
//                } else {
//                    sm->SetStateDuration(offStateDuration.value());
//                }
//            }
//        }
//
//        /// Timer for turning on transition
//        SP<TransformTimer> turnOnTimer =
//            MAKE<TransformTimer>(2.0f, Runner::RunType::RunOnce, MAKE<OutSquaredEase>());
//
//        /// Timer for turning off transition
//        SP<TransformTimer> turnOffTimer =
//            MAKE<TransformTimer>(2.0f, Runner::RunType::RunOnce, MAKE<OutSquaredEase>());
//
//        /// If true, stops turning animations
//        bool isLocked = false;
//
//        float valveState = 0; // (0-1.0) 1.0 is on
//
//    public:
//        float ValveState() const {
//            return valveState;
//        }
//
//        void SetValveState(float value) {
//            if (value == valveState) {
//                return;
//            }
//            valveState = value;
//            OnValveStateChange();
//        }
//
//        Valve() {}
//
//        void OnUpdate(TimeSlice time) override {
//            // (called below): Base::OnUpdate(time);
//
//            if (isLocked) {
//                return;
//            }
//
//            switch (State()) {
//            case StateType::TurnOn:
//                if (turnOnTimer) {
//                    turnOnTimer->OnUpdate(time);
//
//                    SetValveState(turnOnTimer->Progress());
//                    if (turnOnTimer->IsFinished()) {
//                        OnTurnFinish();
//                    }
//                }
//                break;
//            case StateType::TurnOff:
//                if (turnOffTimer) {
//                    turnOffTimer->OnUpdate(time);
//
//                    SetValveState(1.0f - turnOffTimer->Progress());
//                    if (turnOffTimer->IsFinished()) {
//                        OnTurnFinish();
//                    }
//                }
//                break;
//            case StateType::Off:
//            case StateType::On:
//                break;
//            }
//
//            // Do this last because it might cause a state change (would cause
//            // double time event)
//            Base::OnUpdate(time);
//        }
//
//        void OnStateFinish(TimedStateMachine<StateType>& inStateMachine) override {
//            switch (State()) {
//            case StateType::Off:
//                if (turnOnTimer) {
//                    turnOnTimer->Reset();
//                    SetState(StateType::TurnOn);
//                }
//                break;
//            case StateType::On:
//                if (turnOffTimer) {
//                    turnOffTimer->Reset();
//                    SetState(StateType::TurnOff);
//                }
//                break;
//            case StateType::TurnOff:
//            case StateType::TurnOn:
//                break;
//            }
//        }
//
//        void OnStateChange(TimedStateMachine<StateType>& inStateMachine) override {
//            if (&inStateMachine != sm.get()) {
//                return;
//            }
//
//            switch (State()) {
//            case StateType::Off:
//                sm->ResetStateTimer();
//                SetValveState(0);
//                if (offStateDuration) {
//                    sm->SetStateDuration(offStateDuration.value());
//                }
//                break;
//            case StateType::On:
//                sm->ResetStateTimer();
//                SetValveState(1.0f);
//                if (onStateDuration) {
//                    sm->SetStateDuration(onStateDuration.value());
//                }
//                break;
//            case StateType::TurnOn:
//            case StateType::TurnOff:
//                break;
//            }
//        }
//
//        virtual void OnTurnFinish() {
//            switch (State()) {
//            case StateType::TurnOff:
//                SetState(StateType::Off);
//                break;
//            case StateType::TurnOn:
//                SetState(StateType::On);
//                break;
//            case StateType::Off:
//            case StateType::On:
//                break;
//            }
//        }
//
//        void TurnOn(InputEffect inputEffect) {
//            auto isImmediate = inputEffect == InputEffect::Immediate;
//            if (isLocked) {
//                return;
//            }
//
//            if (isImmediate || turnOnTimer == nullptr) {
//                SetState(StateType::On);
//                return;
//            }
//            SetState(StateType::TurnOn);
//            turnOnTimer->SetProgress(ValveState());
//        }
//
//        void TurnOff(InputEffect inputEffect) {
//            auto isImmediate = inputEffect == InputEffect::Immediate;
//            if (isLocked) {
//                return;
//            }
//
//            if (isImmediate || turnOffTimer == nullptr) {
//                SetState(StateType::Off);
//                return;
//            }
//            SetState(StateType::TurnOff);
//            turnOffTimer->SetProgress(1.0f - ValveState());
//        }
//
//        void Switch(InputEffect inputEffect) {
//            if (isLocked) {
//                return;
//            }
//
//            switch (this->State()) {
//            case StateType::Off:
//            case StateType::TurnOff:
//                TurnOn(inputEffect);
//                break;
//            case StateType::On:
//            case StateType::TurnOn:
//                TurnOff(inputEffect);
//                break;
//            }
//        }
//
//        void InputBinary(bool binary, InputEffect inputEffect) {
//            if (binary) {
//                TurnOn(inputEffect);
//            } else {
//                TurnOff(inputEffect);
//            }
//        }
//
//    protected:
//        // OPTIONAL:
//        virtual void OnValveStateChange() {}
//
//        virtual void OnOn() {}
//
//        virtual void OnOff() {}
//    };
//} // namespace PJ
//
// #endif
