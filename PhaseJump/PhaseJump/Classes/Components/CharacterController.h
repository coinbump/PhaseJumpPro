#pragma once

#include "BehaviorNode.h"
#include "StateMachine.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 10/19/24
 */
namespace PJ {
    /// Used to control states and behaviors for a character
    /// Example: game boss with phase states, and behavior trees for each phase of the battle
    class CharacterController : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = CharacterController;
        using StateType = String;

        using BuildBehaviorFunc = std::function<UP<BehaviorNode>(This&)>;

    protected:
        UP<BehaviorNode> behavior;

        /// When the character state changes, the funcs are used to build a behavior
        /// tree for that state
        UnorderedMap<StateType, BuildBehaviorFunc> buildBehaviorFuncs;

    public:
        /// Controller states
        StateMachine<StateType> states;

        CharacterController();

        void AddBuildBehaviorFunc(StateType state, BuildBehaviorFunc func);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "CharacterController";
        }

        // MARK: SomeUpdatable

        void OnUpdate(TimeSlice time) override;
    };
} // namespace PJ
