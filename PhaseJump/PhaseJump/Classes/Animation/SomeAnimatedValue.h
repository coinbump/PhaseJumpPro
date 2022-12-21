#ifndef PJSOMEANIMATEDVALUE_H
#define PJSOMEANIMATEDVALUE_H

#include "SomeMap.h"

namespace PJ
{
    /// <summary>
    /// An animated value changes from 0-1.0 normalized progress
    /// </summary>
    template <class T> class SomeAnimatedValue : public SomeMap<float, T>
    {
    public:
        T ValueFor(float key) const override { return ValueAt(key); }

        virtual T ValueAt(float progress) const = 0;
    };
}

#endif
