#pragma once

#include "Macros.h"
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

    using AssertFunc = std::function<void(bool)>;
    extern AssertFunc assertFunc;

    void Assert(bool isTrue);
} // namespace PJ
