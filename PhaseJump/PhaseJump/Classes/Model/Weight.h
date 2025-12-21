#pragma once

#include "Transformer.h"
#include "Utils.h"
#include "Void.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/2/24
 */
namespace PJ {
    /// Protocol for an object that contains the value type
    template <class Type>
    class SomeValue {
    public:
        virtual ~SomeValue() {};

        virtual Type Value() const = 0;
        virtual void SetValue(Type value) = 0;
    };

    /// A weighted value contains a weight and choice value
    template <class Choice = Void>
    class SomeWeight : public SomeValue<float> {
    public:
        virtual Choice const& GetChoice() const = 0;
    };

    /// Specifies the fixed weight of a random event
    template <class Choice>
    class Weight : public SomeWeight<Choice> {
    public:
        using Base = SomeWeight<Choice>;
        using This = Weight;

    protected:
        float weight{};
        Choice choice;

    public:
        Weight(float weight, Choice choice) :
            choice(choice),
            weight(weight) {}

        // MARK: SomeWeight

        float Value() const override {
            return weight;
        }

        void SetValue(float value) override {
            weight = value;
        }

        Choice const& GetChoice() const override {
            return choice;
        }
    };

    /// Specifies the weight of a random event dynamically
    template <class Choice>
    class DynamicWeight : public SomeWeight<Choice> {
    public:
        using Base = SomeWeight<Choice>;
        using This = DynamicWeight<Choice>;

        using ValueFunc = std::function<float(This const&)>;

    protected:
        ValueFunc weightValueFunc;
        Choice choice;

    public:
        DynamicWeight(ValueFunc weightValueFunc, Choice choice) :
            choice(choice),
            weightValueFunc(weightValueFunc) {}

        // MARK: SomeWeight

        float Value() const override {
            GUARDR(weightValueFunc, 0)
            return weightValueFunc(*this);
        }

        void SetValue(float value) override {
            PJ::Log("ERROR. Weight value is determined dynamically");
        }

        Choice const& GetChoice() const override {
            return choice;
        }
    };
} // namespace PJ
