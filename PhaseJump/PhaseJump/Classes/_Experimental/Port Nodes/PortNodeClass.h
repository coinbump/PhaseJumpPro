#pragma once

#include "Attributes.h"
#include "Class.h"
#include "OrderedMap.h"
#include "TagClass.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    class PortEvaluation;
    class PortNode;

    /// Defines metadata for a port node type
    class PortNodeClass : public Class {
    public:
        using Base = Class;
        using InputPort = TagClass;
        using OutputPort = TagClass;
        using InputMap = OrderedMap<String, UP<InputPort>>;
        using OutputMap = OrderedMap<String, UP<OutputPort>>;

        using EvalFunc = std::function<PortEvaluation(PortNode&)>;

        enum class Type {
            /// Produces data and is only evaluated once
            Data,

            /// Acts like a function call and is forward-evaluated in standard execution flow
            Action
        };

        Type type;
        EvalFunc evalFunc;

        InputMap inputs;
        OutputMap outputs;

        PortNodeClass(String id, Type type, EvalFunc evalFunc) :
            Base({ .id = id }),
            type(type),
            evalFunc(evalFunc) {}

        template <class T>
        InputMap::iterator AddInput(String id, String name, T defaultValue = T()) {
            UP<InputPort> input = NEW<TypeTagClass<T>>(id);
            input->Add(Attributes::NameAttribute(name))
                .Add(Attributes::DefaultValueAttribute<T>(defaultValue));

            return inputs.insert_or_assign(id, std::move(input)).first;
        }

        template <class T>
        OutputMap::iterator AddOutput(String id, String name) {
            UP<OutputPort> output = NEW<TypeTagClass<T>>(id);
            output->Add(Attributes::NameAttribute(name));

            return outputs.insert_or_assign(id, std::move(output)).first;
        }
    };
} // namespace PJ
