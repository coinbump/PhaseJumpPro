#ifndef PJFACTORY_H_
#define PJFACTORY_H_

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
        virtual void* NewObject() = 0;
    };

    /// <summary>
    /// Creates simple type
    /// </summary>
    template <class Type> class Factory : public SomeFactory
    {
    protected:
        std::function<Type()> constructor;

    public:
        Factory(std::function<Type()> constructor) : constructor(constructor)
        {
        }

        Type New()
        {
            return constructor();
        }

        void* NewObject() override
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
        Type* New()
        {
            return new Type();
        }

        void* NewObject() override
        {
            return New();
        }
    };
}

#endif
