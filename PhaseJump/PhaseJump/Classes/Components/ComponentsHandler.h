#pragma once

#include "WorldComponent.h"
#include "WorldNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/8/25
 */
namespace PJ {
    /// When run, it is given all of a node's components except this one
    /// Run must be called manually
    template <typename... Arguments>
    class ComponentsHandler : public WorldComponent {
    public:
        using Func = std::function<FinishType(SomeWorldComponent&, Arguments...)>;

        struct Config {
            String type;
            Func func;
        };

        String type;

        /// Func to run for each component
        Func func;

        ComponentsHandler(Config const& config) :
            type(config.type),
            func(config.func) {}

        void Run(Arguments... args) {
            GUARD(owner)
            GUARD(func)

            auto& components = owner->Components();
            for (auto& component : components) {
                GUARD_CONTINUE(component.get() != this)

                auto finishType = func(*component.get(), args...);
                GUARD_BREAK(finishType == FinishType::Continue)
            }
        }
    };
} // namespace PJ
