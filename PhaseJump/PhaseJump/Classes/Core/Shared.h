#pragma once

#include "Dev.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/10/24
 */
namespace PJ {
    /**
     Stores either a shared object or a modifiable object

     When dealing with large objects,  we want to reduce the # of
     times the data gets copied. This allows an object to be shared
     until the point in time where we need to copy the object in order to modify it

     Example: large mesh
     */
    template <class Type>
    class Shared {
    protected:
        bool isModifiable = false;

        // FUTURE: evaluate using std::monostate if needed
        using Ptr = Type const*;

        // FUTURE: support shared_ptr if needed
        std::variant<Ptr, Type> value;

    public:
        Shared() :
            isModifiable(false) {}

        Shared(Ptr ptr) :
            value(ptr),
            isModifiable(false) {}

        Shared(Type const& typeValue) :
            value(typeValue),
            isModifiable(true) {}

        /// Returns a ptr to the type value
        /// Can either be a ptr a shared typeValue stored somewhere else or a ptr for this object's
        /// typeValue
        constexpr Ptr Value() const {
            auto ptr = std::get_if<Ptr>(&value);
            if (ptr) {
                return *ptr;
            }

            auto typeValue = std::get_if<Type>(&value);
            if (typeValue) {
                return &(*typeValue);
            }

            return nullptr;
        }

        /// Returns true if the stored value is inherantly modifiable (not shared)
        constexpr bool IsModifiable() const {
            return isModifiable;
        }

        /// Returns true if the stored value is shared
        constexpr bool IsShared() const {
            auto ptr = std::get_if<Ptr>(&value);
            return nullptr != ptr;
        }

        /// Shared data can't be modified, so if we are using shared data, we'll need
        /// to copy it into the local value
        Type& Modifiable() {
            if (!isModifiable) {
                isModifiable = true;

                auto typeValue = std::get_if<Type>(&value);
                if (typeValue) {
                    PJ::Log("Error. Storing Value with isModifiable set to false");
                }

                auto ptr = std::get_if<Ptr>(&value);
                if (ptr) {
                    this->value = **ptr;
                }
            }

            return std::get<Type>(value);
        }

        operator Ptr() const {
            return Value();
        }
    };
} // namespace PJ
