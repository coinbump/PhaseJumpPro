#pragma once

#include "EaseFunc.h"
#include "SomeKeyframe.h"
#include "Void.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// Keyframe that stores a value
    template <class Type>
    class ValueKeyframe : public SomeKeyframe {
    public:
        Type value{};
    };

    namespace Keyframes {
        class EaseInOutCore {
        public:
            EaseFunc inEaseFunc;
            EaseFunc outEaseFunc;
        };
    } // namespace Keyframes

    /// Keyframe that stores a value and a core (Example: ease func)
    template <class Type, class Core = Void>
    class Keyframe : public ValueKeyframe<Type> {
    public:
        Core core{};
    };

    template <class Type>
    using EaseKeyframe = Keyframe<Type, Keyframes::EaseInOutCore>;
} // namespace PJ

// FUTURE: using ActionKeyframe = ValueKeyframe<std::function<void()>>;
