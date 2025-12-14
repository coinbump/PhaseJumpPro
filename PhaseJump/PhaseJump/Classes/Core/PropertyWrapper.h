#pragma once

#include "Binding.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/31/24
 */
namespace PJ {
    /**
     Wraps access to a property with funcs. Unlike a binding, it stores the value internally

     Examples:
     - Write properties to local storage as they change, or notify another object
     - Clamp the returned value of a number

     Careful: don't store references to objects that might go out of scope
     */
    template <class T>
    class PropertyWrapper {
    protected:
        T value = T();

    public:
        using GetFunc = std::function<T(T const& value)>;
        using SetFunc = std::function<T(T const& newValue)>;

        GetFunc getFunc;
        SetFunc setFunc;

        PropertyWrapper(GetFunc getFunc, SetFunc setFunc) :
            getFunc(getFunc),
            setFunc(setFunc) {}

        operator T() const {
            return getFunc ? getFunc(value) : value;
        }

        PropertyWrapper& operator=(T const& b) {
            if (setFunc) {
                value = setFunc(b);
            }
            return *this;
        }
    };
} // namespace PJ
