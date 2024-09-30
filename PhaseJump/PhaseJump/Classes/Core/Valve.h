#pragma once

#include "EaseFunc.h"
#include "Timer.h"
#include "Updatable.h"
#include <memory>
#include <optional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    /**
     Common design pattern for interactive media
     A valve transitions between two states over time, usually with an animation
     Example: a switch, or a water valve that opens and closes
     If you need more complex animations (time on hold, time off hold), use the driver pattern to
     drive the valve's states
     */
    class Valve : public Updatable {
    public:
        using Base = Updatable;
        using This = Valve;
        using OnValveUpdateFunc = std::function<void(This&)>;

        enum class StateType {
            /// Valve is off
            Off,

            /// Valve is turning on
            TurnOn,

            /// Valve is on
            On,

            /// Valve is turning off
            TurnOff
        };

    protected:
        StateType state{};
        SP<Timer> timer;
        OnValveUpdateFunc onValveUpdateFunc;

    public:
        Valve(bool isOn = false);

        float Value() const;

        /**
         Turn on the valve in N seconds
         If we are interrupting a previous animation, the actual duration will be proportional to
         the distance left
         */
        void TurnOn(float duration);

        /**
         Turn on the valve in N seconds
         If we are interrupting a previous animation, the actual duration will be proportional to
         the distance left
         */
        void TurnOff(float duration);

        StateType State() const {
            return state;
        }

        bool IsOn() const {
            return state == StateType::On;
        }

        bool IsOff() const {
            return state == StateType::Off;
        }

        void SetOnValveUpdateFunc(OnValveUpdateFunc value) {
            this->onValveUpdateFunc = value;
            OnValveUpdate();
        }

        OnValveUpdateFunc GetOnValveUpdateFunc() const {
            return onValveUpdateFunc;
        }

    protected:
        void SetState(StateType value);
        virtual void OnValveUpdate();
    };
} // namespace PJ
