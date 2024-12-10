#pragma once

#include "InputMap.h"
#include "SomeControllerUIEvent.h"
#include "UnorderedMap.h"
#include "Vector3.h"

// CODE REVIEW: ?/23
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
        static void Update(ControllerAxisUIEvent& event) {
            shared.controllers[event.controllerId].axisValues[event.axisId] = event.value;
        }

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

        //        float
        //        ActionAxisValue(InputMap& inputMap, String negativeAction, String positiveAction,
        //        std::optional<String> controllerId = std::nullopt) const {
        //            auto axisIds = ControllerAxisId::allCases;
        //
        //            std::optional<InputMap::Item> negativeItem;
        //            std::optional<InputMap::Item> positiveItem;
        //
        //            for (auto& axisId : axisIds) {
        //                for (auto& item : inputMap.Items()) {
        //                    if (item.action == negativeAction) {
        //                        if (!negativeItem.has_value()) {
        //                            ControllerAxisUIEvent testEvent("", axisId, 0);
        //                            if (controllerId) {
        //                                testEvent.controllerId = controllerId.value();
        //                            }
        //
        //                            auto firstNegativeFilter = First(item.filters, [=](auto
        //                            filterFunc) { return filterFunc(testEvent); }); if
        //                            (firstNegativeFilter) {
        //                                negativeItem = item;
        //                            }
        //                        }
        //                    } else if (item.action == positiveAction) {
        //
        //                    }
        //                }
        //            }
        //
        //            return 0;
        //        }

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

        // float ActionAxisValue
    };
} // namespace PJ
