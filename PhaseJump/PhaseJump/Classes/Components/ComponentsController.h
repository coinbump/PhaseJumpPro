#pragma once

#include "StateMachine.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/15/24
 */
namespace PJ {
    /// Enables/disables components based on controller states
    /// Example: turn effects on and off based on the current effect state
    class ComponentsController : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = ComponentsController;
        using StateType = String;

        using EnableComponentsFunc = std::function<void(This&, bool)>;

    protected:
        /// Func to enable/disable components based on state
        UnorderedMap<StateType, EnableComponentsFunc> enableComponentsFuncs;

    public:
        /// Controller states
        StateMachine<StateType> states;

        ComponentsController();

        void AddEnableComponentsFunc(StateType state, EnableComponentsFunc func);
        void AddEnableComponentsById(StateType state, UnorderedSet<String> const& ids);
        void AddEnableComponentsByName(StateType state, UnorderedSet<String> const& names);

        template <class Type>
        void AddEnableComponentsByType(StateType state) {
            AddEnableComponentsFunc(state, [](auto& controller, bool enable) {
                for (auto& component : controller.Target()->Components()) {
                    GUARD_CONTINUE(Is<Type>(component.get()))
                    component->Enable(enable);
                }
            });
        }

        template <class Type>
        void AddEnableComponentsByTypeAndId(StateType state, UnorderedSet<String> const& ids) {
            AddEnableComponentsFunc(state, [=](auto& controller, bool enable) {
                for (auto& component : controller.Target()->Components()) {
                    GUARD_CONTINUE(Is<Type>(component.get()))
                    GUARD_CONTINUE(ids.contains(component.id))
                    component->Enable(enable);
                }
            });
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ComponentsController";
        }
    };
} // namespace PJ
