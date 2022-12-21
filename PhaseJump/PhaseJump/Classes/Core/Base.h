#ifndef PJBASE_H
#define PJBASE_H

#include "Types/_String.h"
#include <memory>

/*
 RATING: 5 stars
 Simple base class
 CODE REVIEW: 12/20/22
 */
namespace PJ {
    /// <summary>
    /// Can be converted to a string for logging
    /// </summary>
    class StringConvertable {
        virtual String ToString() const { return "Base"; }

        // Convenience
        String Description() const { return ToString(); }
    };

    /// <summary>
    /// Base class for many framework objects, provides standard behavior and provides polymorphism for factories
    /// so we can use dynamic cast and `shared_from_this`
    /// </summary>
    class Base : public StringConvertable, public std::enable_shared_from_this<Base> {
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
    };
}

#endif
