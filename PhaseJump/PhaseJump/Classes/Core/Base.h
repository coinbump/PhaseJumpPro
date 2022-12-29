#ifndef PJBASE_H
#define PJBASE_H

#include "StringConvertible.h"
#include <memory>

/*
 RATING: 5 stars
 Simple base class
 CODE REVIEW: 12/20/22
 */
namespace PJ {
    /// <summary>
    /// Base class for many framework objects, provides standard behavior and provides polymorphism for factories
    /// so we can use dynamic cast and `shared_from_this`
    /// </summary>
    class Base : public StringConvertible, public std::enable_shared_from_this<Base> {
    protected:
        bool didGo = false;

        virtual void GoInternal() {}

    public:
        virtual ~Base() {}

        // Initiate some behavior
        virtual void Go() {
            if (didGo) { return; }
            didGo = true;

            GoInternal();
        }

        String ToString() const override { return "Base"; }
    };
}

#endif
