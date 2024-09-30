#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

using namespace PJ;

namespace Example {
    namespace Life {
        /// Stores data for the life cell agent
        struct AgentCore {
            /// If true, this cell has life init
            bool isAlive = false;

            /// Step logic is based on current states, don't update state until system step is
            /// finished
            bool nextIsAlive = false;

            /// Cell location for agent
            Vector2Int location;
        };

        using Agent = Agent<AgentCore>;
    } // namespace Life
} // namespace Example
