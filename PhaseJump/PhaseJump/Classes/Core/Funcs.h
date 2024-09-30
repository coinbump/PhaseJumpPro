#pragma once

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 9/7/24
 */
namespace PJ {
    /*
     Benchmarking info

     100,000 runs (append 1 character to string):
     For loop: 393.201ms
     Func For: 390.999ms
     Func For without constexpr: 403.128ms:
     */
    /// Repeats the func N times
    template <class Count, class Func>
    constexpr void Repeat(Count count, Func func) {
        for (Count i = 0; i < count; i++) {
            func();
        }
    }

    /// Repeats the func N times and sends a count
    template <class Count, class Func>
    constexpr void RepeatCount(Count count, Func func) {
        for (Count i = 0; i < count; i++) {
            func(i);
        }
    }

    /// Repeats the func in a loop from start to end
    template <class Count, class Func>
    constexpr void For(Count start, Count end, Func func) {
        for (Count i = start; i < end; i++) {
            func();
        }
    }

    /// Repeats the func in a loop from start to end, and sends a count
    template <class Count, class Func>
    constexpr void ForCount(Count start, Count end, Func func) {
        for (Count i = start; i < end; i++) {
            func(i);
        }
    }

    /// Creates an override func that calls both the original func and the new one
    template <class Func>
    constexpr Func OverrideFunc(Func const& baseFunc, Func const& func) {
        GUARDR(baseFunc, func)
        GUARDR(func, baseFunc)

        // Using && moves the arguments instead of copying them
        return [=](auto&&... args) {
            baseFunc(args...);
            func(args...);
        };
    }

    /// Overrides a base func with  new func, calling the base func first in the override
    template <class Func>
    constexpr void Override(Func& baseFunc, Func const& func) {
        baseFunc = OverrideFunc(baseFunc, func);
    }

#ifdef PROFILE
    /// Not meant to be generally used. Just for profiling
    template <class Count, class Func>
    void ForNoConstexpr(Count start, Count end, Func func) {
        for (Count i = start; i < end; i++) {
            func();
        }
    }
#endif
} // namespace PJ
