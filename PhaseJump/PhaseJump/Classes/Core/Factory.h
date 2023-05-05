#ifndef PJFACTORY_H_
#define PJFACTORY_H_

#include "Base.h"
#include "AnyFactory.h"
#include "Macros.h"
#include <functional>

/*
 RATING: 5 stars
 Utility with unit tests
 CODE REVIEW: 11/7/22
 */
namespace PJ
{
    class SomeFactory : public AnyFactory
    {
    public:
        virtual SP<Base> NewObject() = 0;
    };

    /// <summary>
    /// Creates simple type
    /// </summary>
    template <class Type>
    class Factory : public SomeFactory
    {
    protected:
        std::function<SP<Type>()> allocator;

    public:
        Factory(std::function<SP<Type>()> allocator) : allocator(allocator)
        {
        }

        SP<Type> New()
        {
            return allocator();
        }

        SP<Base> NewObject() override
        {
            return New();
        }
    };

    /// <summary>
    /// Convenience class
    /// </summary>
    template <class Type> class FactoryNew : public SomeFactory
    {
    public:
        SP<Type> New()
        {
            return MAKE<Type>();
        }

        SP<Base> NewObject() override
        {
            return New();
        }
    };
}

#endif
