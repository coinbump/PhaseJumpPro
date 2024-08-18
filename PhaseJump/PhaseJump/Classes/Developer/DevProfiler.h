#pragma once

namespace PJ {
    struct DevProfiler {
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
        using OnFinishFunc = std::function<void(String)>;

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
            if (durationInMicroseconds > 0) {
                ss << name << " profile: ";
                ss << durationInMicroseconds / 1000.0 << "ms:";
                ss << std::endl;
            }

            GUARD(onFinishFunc)
            onFinishFunc(ss.str());
        }
    };
} // namespace PJ
