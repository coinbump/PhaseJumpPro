#pragma once

#include "History.h"
#include "StringUtils.h"
#include <functional>
#include <mutex>
#include <sstream>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/19/26
 */
namespace PJ {
    /// Thread-safe buffer of log strings
    class DevLog {
    protected:
        History<String> history{ 100 };

    public:
        using This = DevLog;
        using OnAddFunc = std::function<void(String const&)>;

        /// Called after a string is added. Useful for UI auto-scroll
        OnAddFunc onAddFunc;

        DevLog();

        void Add(String value);

        History<String> const& GetHistory() const {
            return history;
        }

        template <class T>
        DevLog& operator<<(T const& value) {
            std::stringstream stream;
            stream << value;
            Add(stream.str());
            return *this;
        }

    protected:
        std::mutex mutex;
    };

    extern DevLog devLog;
} // namespace PJ
