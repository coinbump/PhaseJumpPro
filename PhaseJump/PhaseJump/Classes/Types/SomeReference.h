#ifndef PJSOMEREFERENCE_H
#define PJSOMEREFERENCE_H

#include "Macros.h"
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
        
        virtual SP<T> Value() const = 0;
        virtual void SetValue(SP<T> const& value) = 0;
    };
}

#endif
