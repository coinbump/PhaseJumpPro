#pragma once

#include "Macros.h"
#include "SomeBinding.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    template <class T>
    using SetBindingFunc = std::function<void(T const&)>;

    /// Binding that gets and sets values via function
    template <class T>
    class Binding : public SomeBinding<T> {
    public:
        using GetFunc = std::function<T()>;
        using SetFunc = SetBindingFunc<T>;

        GetFunc getFunc;
        SetFunc setFunc;

        Binding(GetFunc getFunc, SetFunc setFunc) :
            getFunc(getFunc),
            setFunc(setFunc) {}

        T Value() override {
            GUARDR(getFunc, T())
            return getFunc();
        }

        void SetValue(T const& value) override {
            GUARD(setFunc)
            setFunc(value);
        }
    };
} // namespace PJ
