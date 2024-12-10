#pragma once

#include "StateMachine.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/19/24
 */
namespace PJ {
    class SomeRenderer;

    /// Manages animation states and switches to the appropriate renderer when the state changes
    /// Animation transitions are not supported
    class SimpleAnimationController : public WorldComponent<> {
    public:
        using This = SimpleAnimationController;
        using StateType = String;

        using BuildRendererFunc = std::function<SP<SomeRenderer>(This&)>;
        using SetRendererFunc = std::function<void(This&, SP<SomeRenderer>)>;

    protected:
        /// Build renderer funcs for each animation state
        UnorderedMap<String, BuildRendererFunc> buildRendererFuncs;

    public:
        // FUTURE: Implement AnimationController for animations with transitions if needed
        // AnimationController animationController;

        /// Animation states
        StateMachine<StateType> states;

        /// Called to replace the node's renderer when the animation state changes
        SetRendererFunc setRendererFunc;

        SimpleAnimationController();

        virtual ~SimpleAnimationController() {}

        void Add(String state, BuildRendererFunc buildRendererFunc);

        StateType State() const {
            return states.State();
        }

        void SetState(
            StateType value, PublishValueCondition condition = PublishValueCondition::OnChange
        ) {
            states.SetState(value, condition);
        }

        void Input(StateType value) {
            states.Input(value);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SimpleAnimationController";
        }
    };
} // namespace PJ
