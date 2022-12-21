#ifndef PJSOMEBINDING_H
#define PJSOMEBINDING_H

#include <functional>

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 11/26/22
 */
namespace PJ
{
    /// <summary>
    /// Type erasure interface
    /// </summary>
    class AnyBinding
    {
    };

    /// <summary>
    /// Wrapper used to alter the attribute of an object
    /// </summary>
    template <class T>
    class SomeBinding : public AnyBinding
    {
    public:
        virtual T Value() = 0;
        virtual void SetValue(T const& value) = 0;
    };
}

#endif
