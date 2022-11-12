#ifndef PJBASE_H
#define PJBASE_H

/*
 * RATING: 5 stars. Simple base class.
 * CODE REVIEW: 11/7/22
 */
namespace PJ {
    /// <summary>
    /// Base class for many framework objects, provides standard behavior and provides polymorphism for factories
    /// so we can use dynamic cast
    /// </summary>
    class Base {
    public:
        virtual ~Base() {}

        virtual void Go() {}    // Initiate some behavior
    };
}

#endif
