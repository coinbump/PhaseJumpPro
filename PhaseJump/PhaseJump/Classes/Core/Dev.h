#pragma once

#include <assert.h>
#include <functional>
#include <iostream>
#include <stdio.h>

namespace PJ {
#ifdef VERBOSE
    template <class... Arguments>
    void Log(Arguments... args) {
        ([&] { std::cout << args; }(), ...);
        std::cout << std::endl;
    }
#else
    template <class... Arguments>
    void Log(Arguments... args) {}
#endif

    struct Asserter {
        using AssertFunc = std::function<void(bool isTrue)>;
        AssertFunc assertFunc;

        void Assert(bool isTrue);
    };

    extern Asserter asserter;

#ifdef DEBUG
    void Assert(bool isTrue);
#else
    void Assert(bool isTrue);
#endif
} // namespace PJ
