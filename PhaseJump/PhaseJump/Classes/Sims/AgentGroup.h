#pragma once

#include "Agent.h"
#include "Pool.h"
#include "Updatable.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/8/24
 */
namespace PJ {
    class AgentSystem;

    /// Allows templated agent groups to be stored in collections
    class AgentGroup : public SomeUpdatable {
    protected:
        float stepTimer{};

    public:
        /// System that owns this group
        AgentSystem* system{};

        /// (Optional). For step-based simulations, the time value between steps (0 if steps are
        /// disabled)
        float stepTime{};
    };

    /// Handles updates and collection logic for a group of agents
    template <class AgentType = Agent<>>
    class TypeAgentGroup : public AgentGroup {
    public:
        using Agent = AgentType;
        using AgentPool = Pool<Agent>;

    protected:
        AgentPool agents;
        Updatable updatable;

    public:
        AgentPool const& Agents() const {
            return agents;
        }

        TypeAgentGroup(size_t size = 100) :
            agents(size, 0) {
            updatable.onUpdateFunc = [this](auto& updatable, auto time) {
                // Give all agents update delta
                RunAgentsAction([=](auto& agent) { agent.OnUpdate(time); });

                // For step-based simulation give all agents steps
                if (stepTime > 0) {
                    stepTimer += time.delta;
                    while (stepTimer >= stepTime) {
                        RunAgentsAction([](auto& agent) { agent.OnStep(); });
                        PostStep();
                        stepTimer -= stepTime;
                    }
                }

                return FinishType::Continue;
            };
        }

        /// Adds agent to the pool or reuses it
        /// Important: don't store pointers to objects inside resizable pools, memory will move
        Agent* Add() {
            auto agent = agents.Add();
            GUARDR(agent, nullptr)
            agent->group = this;

            return agent;
        }

        /// Run a function for all active agents
        void RunAgentsAction(std::function<void(Agent&)> action) {
            for (size_t i = 0; i <= agents.LastActiveIndex(); i++) {
                try {
                    auto& agent = agents.at(i);
                    GUARD(agent.IsActive())
                    action(agent);
                } catch (...) {
                    return;
                }
            }
        }

        /// (Optional). After each step, do any needed logic
        virtual void PostStep() {}

        // MARK: SomeUpdatable

        FinishType OnUpdate(TimeSlice time) override {
            return updatable.OnUpdate(time);
        }

        bool IsFinished() const override {
            return updatable.IsFinished();
        }
    };
} // namespace PJ
