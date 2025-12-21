#pragma once

#include "Class.h"
#include "Factory.h"
#include "FactoryRegistry.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Provides a default factory object to create objects of the related type
    template <class Type, typename... Arguments>
    class TypeClass : public Class {
    public:
        using Base = Class;
        using FactoryFunc = std::function<UP<Type>(Arguments... args)>;

        /// Factory to produce an object of this class type
        FactoryFunc factoryFunc;

        TypeClass(String id, FactoryFunc factoryFunc) :
            Base({ .id = id }),
            factoryFunc(factoryFunc) {}

        UP<Type> New(Arguments... args) {
            GUARDR(factoryFunc, nullptr)
            return factoryFunc(args...);
        }

        SP<Type> Make(Arguments... args) {
            return std::move(New(args...));
        }
    };
} // namespace PJ
