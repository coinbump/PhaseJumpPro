#pragma once

#include "Macros.h"
#include "StringUtils.h"
#include <assert.h>
#include <functional>
#include <iostream>
#include <stdio.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/15/24
 */
namespace PJ {
    /// Specifies how much "noise" logging should make
    namespace LogLevel {
        /// No logging at all
        auto constexpr Quiet = "quiet";

        /// Log everything
        auto constexpr Verbose = "verbose";
    } // namespace LogLevel

    extern String logLevel;

    bool IsLogVerbose();

    template <class... Arguments>
    void Log(Arguments... args) {
        if (logLevel == LogLevel::Verbose) {
            ([&] { std::cout << args; }(), ...);
            std::cout << std::endl;
        }
    }

    template <class... Arguments>
    void LogIf(bool check, Arguments... args) {
        GUARD(check)
        Log(args...);
    }

    using AssertFunc = std::function<void(bool)>;
    extern AssertFunc assertFunc;

    void Assert(bool isTrue);
} // namespace PJ

// MARK: Macros

#define GUARD_LOG(test, message) \
    if (!(test)) {               \
        PJ::Log(message);        \
        return;                  \
    }

#define GUARDR_LOG(test, result, message) \
    if (!(test)) {                        \
        PJ::Log(message);                 \
        return result;                    \
    }
