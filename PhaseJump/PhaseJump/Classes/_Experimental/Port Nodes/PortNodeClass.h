#pragma once

#include "Attributes.h"
#include "Class.h"
#include "SomeTagClass.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    class PortEvaluation;
    class PortNode;

    /// Defines metadata for a port node type
    class PortNodeClass : public Class<> {
    public:
        using Base = Class<>;
        using InputPort = SomeTagClass;
        using OutputPort = SomeTagClass;
        using EvalFunc = std::function<PortEvaluation(PortNode&)>;

        enum class Type {
            /// Produces data and is only evaluated once
            Data,

            /// Acts like a function call and is forward-evaluated in standard execution flow
            Action
        };

        Type type;
        EvalFunc evalFunc;

        OrderedMap<String, SP<InputPort>> inputs;
        OrderedMap<String, SP<OutputPort>> outputs;

        PortNodeClass(String id, Type type, EvalFunc evalFunc) :
            Base(id),
            type(type),
            evalFunc(evalFunc) {}

        template <class T>
        InputPort& AddInput(String id, String name, T defaultValue = T()) {
            auto input = MAKE<TagClass<T>>(id);
            input->Add(Attributes::NameAttribute(name))
                .Add(Attributes::DefaultValueAttribute<T>(defaultValue));
            inputs[id] = input;

            return *input;
        }

        template <class T>
        OutputPort& AddOutput(String id, String name) {
            auto output = MAKE<TagClass<T>>(id);
            output->Add(Attributes::NameAttribute(name));
            outputs[id] = output;

            return *output;
        }
    };
} // namespace PJ
