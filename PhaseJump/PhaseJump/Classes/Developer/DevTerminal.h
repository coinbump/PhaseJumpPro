#pragma once

#include "History.h"
#include "StringUtils.h"
#include <functional>
#include <mutex>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/19/26
 */
namespace PJ {
    /// Thread safe command processor
    /// The default runCommandFunc echoes the command to the log
    class DevTerminal {
    protected:
        History<String> history{ 100 };

    public:
        using This = DevTerminal;
        using RunCommandFunc = std::function<void(DevTerminal&, String const&)>;

        /// Called whenever a command is submitted. Replace to customize behavior
        RunCommandFunc runCommandFunc;

        DevTerminal();

        History<String> const& GetHistory() const {
            return history;
        }

        void RunCommand(String command);

    protected:
        std::mutex mutex;
    };

    extern DevTerminal devTerminal;
} // namespace PJ
