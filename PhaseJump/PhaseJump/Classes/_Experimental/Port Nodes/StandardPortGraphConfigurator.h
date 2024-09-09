#pragma once

#include "PortEvaluation.h"
#include "PortNode.h"
#include "PortNodeClass.h"

namespace PJ {
    class SomeAttribute;

    namespace PortGraphAttributeId {
        auto constexpr NoPort = "portGraph.noPort";
    } // namespace PortGraphAttributeId

    namespace PortGraphNodeClassId {
        auto constexpr VarFloat = "var.float";
        auto constexpr MultiplyFloat = "mult.float";
    } // namespace PortGraphNodeClassId

    namespace PortGraphAttributes {
        SP<SomeAttribute> HidePortAttribute();
    }

    struct StandardPortGraphConfigurator {
        void Configure() {
            ConfigureVarFloat();
        }

        void ConfigureVarFloat() {
            // TODO: test with simple nodes: float var/const -> Mult -> cout processor and then done
            // (POC)
            //            SP<PortNodeClass> varFloatClass = MAKE<PortNodeClass>(
            //                PortGraphNodeClassIds::varFloat, PortNodeClass::Type::Data,
            //                [](PortNode& node) {
            //                    float inputValue = node.SafeInputValue<float>("value");
            //
            //                    PortEvaluation result;
            //                    result.tags["value"] = inputValue;
            //                    return result;
            //                }
            //            );
            //
            //            varFloatClass->AddInput<float>("value", "Value")
            //                .Add(PortGraphAttributes::HidePortAttribute());
            //            varFloatClass->AddOutput<float>("value", "Value");
        }

        void ConfigureMultFloat() {
            SP<PortNodeClass> result = MAKE<PortNodeClass>(
                PortGraphNodeClassId::MultiplyFloat, PortNodeClass::Type::Data,
                [](PortNode& node) {
                    //                float inputValue = node.SafeInputValue<float>("value");
                    //                float factor = node.SafeInputValue<float>("factor");

                    PortEvaluation result;
                    //                result.tags["value"] = inputValue * factor;
                    return result;
                }
            );

            result->AddInput<float>("value", "Value");
            result->AddInput<float>("factor", "Factor");
            result->AddOutput<float>("value", "Value");
        }
    };
} // namespace PJ
