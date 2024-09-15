#pragma once

#include "StringUtils.h"

/*
 RATING: 5 stars
 Tested and works (dev only)
 CODE REVIEW: 9/3/24
 */
namespace PJ {
    /**
    Starts profiling in its constructor and calls the provided func in its destructor with the
    profile result
     */
    struct DevProfiler {
        struct Result {
            String formattedString;
            double durationInMilliseconds = 0;

            Result(String formattedString, double durationInMilliseconds) :
                formattedString(formattedString),
                durationInMilliseconds(durationInMilliseconds) {}
        };

        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
        using OnFinishFunc = std::function<void(Result)>;

        String name;
        TimePoint start;
        OnFinishFunc onFinishFunc;

        DevProfiler(String name, OnFinishFunc onFinishFunc) :
            name(name),
            onFinishFunc(onFinishFunc) {
            start = std::chrono::steady_clock::now();
        }

        virtual ~DevProfiler() {
            std::ostringstream ss;

            auto now = std::chrono::steady_clock::now();
            auto duration = now - start;
            double durationInMicroseconds =
                std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

            double durationInMilliseconds = 0;
            if (durationInMicroseconds > 0) {
                durationInMilliseconds = durationInMicroseconds / 1000.0;

                ss << name << " profile: ";
                ss << durationInMilliseconds << "ms:";
                ss << std::endl;
            }

            GUARD(onFinishFunc)

            Result result(ss.str(), durationInMilliseconds);
            result.formattedString = ss.str();
            onFinishFunc(result);
        }
    };

    /**
     Runs two funcs N times based on runCount and sends the timed results
     Used for A/B profiling of competing algorithms

     Other profiling tools:
     QuickBench: https://quick-bench.com/
     Tracy Profiler: https://github.com/wolfpld/tracy/releases
     RenderDoc: https://renderdoc.org
     Compiler Explorer: https://godbolt.org
     */
    struct DevABProfiler {
        using Result = DevProfiler::Result;
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
        using Func = std::function<void()>;
        using OnFinishFunc = std::function<void(Result)>;

        String nameA;
        Func aFunc;
        String nameB;
        Func bFunc;
        TimePoint start;
        int runCount = 1;
        OnFinishFunc onFinishFunc;

        DevABProfiler(
            String nameA, Func aFunc, String nameB, Func bFunc, int runCount,
            OnFinishFunc onFinishFunc
        ) :
            nameA(nameA),
            aFunc(aFunc),
            nameB(nameB),
            bFunc(bFunc),
            runCount(runCount),
            onFinishFunc(onFinishFunc) {

            start = std::chrono::steady_clock::now();

            for (int i = 0; i < runCount; i++) {
                aFunc();
            }
            Finish(nameA);

            start = std::chrono::steady_clock::now();

            for (int i = 0; i < runCount; i++) {
                bFunc();
            }
            Finish(nameB);
        }

        void Finish(String name) {
            std::ostringstream ss;

            auto now = std::chrono::steady_clock::now();
            auto duration = now - start;
            double durationInMicroseconds =
                std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
            double durationInMilliseconds = 0;
            if (durationInMicroseconds > 0) {
                durationInMilliseconds = durationInMicroseconds / 1000.0;
                ss << name << " profile: ";
                ss << durationInMilliseconds << "ms:";
                ss << std::endl;
            }

            GUARD(onFinishFunc)

            Result result(ss.str(), durationInMilliseconds);
            result.formattedString = ss.str();

            onFinishFunc(result);
        }
    };
} // namespace PJ
