#pragma once

#include "AnyFactory.h"
#include "Base.h"
#include "Utils.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/20/24
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

        SP<BaseType> MakeBase(Arguments... args) {
            return std::move(NewBase(args...));
        }

        /// Create an object that subclasses BaseType
        virtual UP<BaseType> NewBase(Arguments... args) = 0;
    };

    /// Uses allocator function for factory instantation
    template <class Type, typename... Arguments>
    class Factory : public SomeFactory<typename Type::RootBaseType, Arguments...> {
    protected:
        using BaseType = Type::RootBaseType;

        std::function<UP<Type>(Arguments... args)> allocator;

    public:
        Factory(std::function<UP<Type>(Arguments... args)> allocator) :
            allocator(allocator) {}

        UP<Type> New(Arguments... args) {
            GUARDR(allocator, {})
            return allocator(args...);
        }

        SP<Type> Make(Arguments... args) {
            return std::move(New(args...));
        }

        // MARK: SomeFactory

        UP<BaseType> NewBase(Arguments... args) override {
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
