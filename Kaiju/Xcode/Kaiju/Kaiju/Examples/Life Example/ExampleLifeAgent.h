#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

using namespace PJ;

namespace ExampleLife {
    /// Stores data for the life cell agent
    struct AgentCore {
        /// If true, this cell has life init
        bool isAlive = false;

        /// Cell location for agent
        Vector2Int location;
    };

    using Agent = Agent<AgentCore>;
}
