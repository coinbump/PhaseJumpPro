#pragma once

#include <PhaseJump/PhaseJump.h>

using namespace PJ;

/// Stores data for the life cell agent
struct ExampleLifeAgentCore {
    /// If true, this cell has life init
    bool isAlive = false;

    /// Cell location for agent
    Vector2Int location;
};

using ExampleLifeAgent = Agent<ExampleLifeAgentCore>;
