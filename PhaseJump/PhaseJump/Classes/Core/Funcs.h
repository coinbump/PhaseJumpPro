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
    template <class Count, class Func>
    constexpr void Repeat(Count count, Func func) {
        for (Count i = 0; i < count; i++) {
            func();
        }
    }

    template <class Count, class Func>
    constexpr void RepeatCount(Count count, Func func) {
        for (Count i = 0; i < count; i++) {
            func(i);
        }
    }

    template <class Count, class Func>
    constexpr void For(Count start, Count end, Func func) {
        for (Count i = start; i < end; i++) {
            func();
        }
    }

    template <class Count, class Func>
    constexpr void ForCount(Count start, Count end, Func func) {
        for (Count i = start; i < end; i++) {
            func(i);
        }
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
