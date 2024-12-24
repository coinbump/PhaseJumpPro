#pragma once

#include "InputMap.h"
#include "SomeControllerUIEvent.h"
#include "UnorderedMap.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Stores state of a controller
    class ControllerModel {
    public:
        using AxisId = String;

        UnorderedMap<AxisId, float> axisValues;
    };

    /// Singleton with global state of input devices
    class Input {
    public:
        using ControllerId = String;
        using AxisId = String;

    protected:
        static Input shared;

        UnorderedMap<ControllerId, ControllerModel> controllers;

    public:
        /**
         Thread safety: this should only be called from the main thread
         */
        static void Update(ControllerAxisUIEvent& event) {
            shared.controllers[event.controllerId].axisValues[event.axisId] = event.value;
        }

        /// @return Returns axis value for a specific controller
        static float AxisValue(ControllerId controllerId, AxisId axisId) {
            auto i = shared.controllers.find(controllerId);
            GUARDR(i != shared.controllers.end(), 0)

            auto& axisValues = i->second.axisValues;
            auto j = axisValues.find(axisId);
            GUARDR(j != axisValues.end(), 0)

            return j->second;
        }

        /// @return Returns axis value for the first controller
        static float FirstAxisValue(AxisId axisId) {
            for (auto& controllerItem : shared.controllers) {
                auto& axisValues = controllerItem.second.axisValues;
                auto j = axisValues.find(axisId);
                if (j != axisValues.end()) {
                    return j->second;
                }
            }
            return 0;
        }

        static Vector3 Accelerometer() {
            // FUTURE: support if needed
            return {};
        }

        static Vector3 Gravity() {
            // FUTURE: support if needed
            return {};
        }

        static Vector3 Magnetometer() {
            // FUTURE: support if needed
            return {};
        }

        static Vector3 Gyroscope() {
            // FUTURE: support if needed
            return {};
        }
    };
} // namespace PJ
