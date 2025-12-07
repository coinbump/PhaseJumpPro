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

    template <class T>
    using GetFunc = std::function<T()>;

    /// Binding that gets and sets values via function
    template <class T>
    class Binding : public SomeBinding<T> {
    public:
        using Base = SomeBinding<T>;
        using This = Binding;

        using GetFunc = GetFunc<T>;
        using SetFunc = SetBindingFunc<T>;

        struct Config {
            GetFunc getFunc;
            SetFunc setFunc;
        };

        GetFunc getFunc;
        SetFunc setFunc;

        Binding() {}

        Binding(Config const& config) :
            getFunc(config.getFunc),
            setFunc(config.setFunc) {}

        Binding(GetFunc getFunc, SetFunc setFunc) :
            getFunc(getFunc),
            setFunc(setFunc) {}

        T Value() const override {
            GUARDR(getFunc, T())
            return getFunc();
        }

        void SetValue(T const& value) const override {
            GUARD(setFunc)
            setFunc(value);
        }

        operator T() const {
            return Value();
        }

        This const& operator=(T const& value) const {
            SetValue(value);
            return *this;
        }

        bool IsValid() const {
            return getFunc || setFunc;
        }
    };
} // namespace PJ
