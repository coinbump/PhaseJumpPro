#ifndef PJFACTORY_H_
#define PJFACTORY_H_

#include "Base.h"
#include "AnyFactory.h"
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
        virtual std::shared_ptr<Base> NewObject() = 0;
    };

    /// <summary>
    /// Creates simple type
    /// </summary>
    template <class Type> class Factory : public SomeFactory
    {
    protected:
        std::function<std::shared_ptr<Type>()> allocator;

    public:
        Factory(std::function<std::shared_ptr<Type>()> allocator) : allocator(allocator)
        {
        }

        std::shared_ptr<Type> New()
        {
            return allocator();
        }

        std::shared_ptr<Base> NewObject() override
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
        std::shared_ptr<Type> New()
        {
            return std::make_shared<Type>();
        }

        std::shared_ptr<Base> NewObject() override
        {
            return New();
        }
    };
}

#endif
