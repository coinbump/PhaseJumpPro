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
    /// Defines the default behavior of the time track
    enum KeyedTimeType {
        /// Key frame values are interpolated
        Interpolate,

        /// Each key frame value is a distinct value
        /// Example: frame id in an animation
        Discrete
    };

    template <class Type>
    struct ValueKeyframeConfig {
        Type value{};
        EaseFunc inEaseFunc;
        EaseFunc outEaseFunc;
    };

    /// Keyframe that stores a value
    template <class Type>
    class ValueKeyframe : public SomeKeyframe {
    protected:
        /// Keep hidden. LLVM can't resolve this type externally
        using Config = ValueKeyframeConfig<Type>;

    public:
        Type value{};

        /// (Optional). Ease func into this value
        EaseFunc inEaseFunc;

        /// (Optional). Ease func from this value
        EaseFunc outEaseFunc;

        ValueKeyframe(Config const& config = {}) :
            value(config.value),
            inEaseFunc(config.inEaseFunc),
            outEaseFunc(config.outEaseFunc) {}

        Type Value() const {
            return value;
        }

        void SetValue(Type _value) {
            value = _value;
        }
    };

    /// Keyframe that stores a value and a core (Example: ease func)
    template <class Type, class Core = Void>
    class Keyframe : public ValueKeyframe<Type> {
    public:
        using Base = ValueKeyframe<Type>;
        using This = Keyframe;

        Core core{};

        Keyframe(Base::Config const& config = {}) :
            Base(config) {}
    };
} // namespace PJ

// FUTURE: using ActionKeyframe = ValueKeyframe<std::function<void()>>;
