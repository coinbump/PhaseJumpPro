#ifndef PJSOMEREFERENCE_H
#define PJSOMEREFERENCE_H

#include <memory>
#include <optional>

/*
 RATING: 5 stars
 Simple interface with unit tests
 CODE REVIEW: 11/16/22
 */
namespace PJ
{
    /// <summary>
    /// Interface for a reference type. Allows either strong or weak reference to value
    /// </summary>
    template <class T>
    struct SomeReference
    {
        virtual ~SomeReference() {}
        
        virtual std::shared_ptr<T> Value() const = 0;
        virtual void SetValue(std::shared_ptr<T> const& value) = 0;
    };
}

#endif
