#ifndef PJBINDING_H
#define PJBINDING_H

#include "SomeBinding.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/12/24
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
            return getFunc();
        }

        void SetValue(T const& value) override {
            setFunc(value);
        }
    };
} // namespace PJ

#endif
