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

        TypeAgentGroup(size_t size, AgentPool::ResetFunc resetFunc) :
            agents(size, resetFunc) {
            updatable.onUpdateFunc = [this](auto& updatable, auto time) {
                // Give all agents update delta
                ForAgents([=](auto& agent) { agent.OnUpdate(time); });

                // For step-based simulation give all agents steps
                if (stepTime > 0) {
                    stepTimer += time.delta;
                    while (stepTimer >= stepTime) {
                        ForAgents([](auto& agent) { agent.OnStep(); });
                        PostStep();
                        stepTimer -= stepTime;
                    }
                }

                return FinishType::Continue;
            };
        }

        /// Adds agent to the pool or reuses it
        /// Important: don't store pointers to objects inside resizable pools, memory will move
        std::optional<typename AgentPool::Getter> Add() {
            auto getter = agents.Add();
            GUARDR(getter, {})
            getter->Get().group = this;

            return getter;
        }

        /// Run a function for all active agents
        void ForAgents(std::function<void(Agent&)> action) {
            agents.ForActive(action);
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
