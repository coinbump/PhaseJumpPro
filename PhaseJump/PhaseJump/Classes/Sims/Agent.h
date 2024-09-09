#pragma once

#include "Pool.h"
#include "TimeSlice.h"
#include "Void.h"
#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    template <class Agent>
    class AgentGroup;

    /// Represents a single agent in an agent sytem
    /// Each agent belongs to a group, which manages the storage and lifecycle of the agent
    template <class Core = Void>
    class Agent : public Poolable {
    public:
        using This = Agent<Core>;
        using OnUpdateFunc = std::function<void(This&, TimeSlice)>;
        using OnStepFunc = std::function<void(This&)>;
        using Group = AgentGroup<This>;

    public:
        Group* group = nullptr;
        Core core;

        OnStepFunc onStepFunc;
        OnUpdateFunc onUpdateFunc;

        Agent() {
            resetFunc = [](auto& agent) { ((This*)&agent)->core = Core(); };
        }

        void OnUpdate(TimeSlice time) {
            GUARD(onUpdateFunc);
            onUpdateFunc(*this, time);
        }

        /// (Optional). Step-based agents operate on fixed time steps
        void OnStep() {
            GUARD(onStepFunc);
            onStepFunc(*this);
        }
    };
} // namespace PJ
