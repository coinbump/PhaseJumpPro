#pragma once

#include "Binding.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/31/24
 */
namespace PJ {
    /// Wraps access to a property with funcs
    /// Example: write properties to local storage as they change, or notify another object
    /// Example: clamp the returned value of a number
    /// Careful: don't store references to objects that might go out of scope
    template <class T>
    class PropertyWrapper {
    protected:
        T value = T();

    public:
        using GetFunc = std::function<T(T const& value)>;
        using UpdateFunc = std::function<T(T const& newValue)>;

        GetFunc getFunc;
        UpdateFunc updateFunc;

        PropertyWrapper(GetFunc getFunc, UpdateFunc updateFunc) :
            getFunc(getFunc),
            updateFunc(updateFunc) {}

        operator T() const {
            return getFunc ? getFunc(value) : value;
        }

        PropertyWrapper& operator=(T const& b) {
            if (updateFunc) {
                value = updateFunc(b);
            }
            return *this;
        }
    };
} // namespace PJ
