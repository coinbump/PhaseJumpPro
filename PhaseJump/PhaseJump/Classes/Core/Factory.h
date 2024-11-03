#pragma once

#include "AnyFactory.h"
#include "Base.h"
#include "Utils.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/10/24
 */
namespace PJ {
    /**
     Associates factory by its root base type

     Allows factories of different types to be stored together
     */
    template <class BaseType = Base, typename... Arguments>
    class SomeFactory : public AnyFactory {
    public:
        virtual ~SomeFactory() {}

        /// Create an object that subclasses BaseType
        virtual SP<BaseType> NewBase(Arguments... args) = 0;
    };

    /// Uses allocator function for factory instantation
    template <class Type, typename... Arguments>
    class Factory : public SomeFactory<typename Type::RootBaseType, Arguments...> {
    protected:
        using BaseType = Type::RootBaseType;

        std::function<SP<Type>(Arguments... args)> allocator;

    public:
        Factory(std::function<SP<Type>(Arguments... args)> allocator) :
            allocator(allocator) {}

        // TODO: SP-audit (Make vs New)
        SP<Type> New(Arguments... args) {
            GUARDR(allocator, SP<Type>())
            return allocator(args...);
        }

        // MARK: SomeFactory

        SP<BaseType> NewBase(Arguments... args) override {
            return New(args...);
        }
    };

    /// An object that provides a type, with arguments
    template <typename Result, typename... Arguments>
    class SomeProvider {
    public:
        virtual ~SomeProvider() {}

        virtual void Provide(Result& result, Arguments... args) = 0;
    };

    /// An object that provides a type, with no arguments
    template <class Result>
    class SomeProvider<void, Result> {
    public:
        virtual ~SomeProvider() {}

        virtual void Provide(Result& result) = 0;
    };

} // namespace PJ
