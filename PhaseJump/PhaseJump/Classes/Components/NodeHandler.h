#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/8/25
 */
namespace PJ {
    /// When run, is given the owner node
    /// Run must be called manually
    template <typename... Arguments>
    class NodeHandler : public WorldComponent<> {
    public:
        using Func = std::function<void(WorldNode&, Arguments...)>;

        struct Config {
            String type;
            Func func;
        };

        String type;

        /// Func to run for owner node
        Func func;

        NodeHandler(Config config) :
            type(config.type),
            func(config.func) {}

        void Run(Arguments... args) {
            GUARD(owner)
            GUARD(func)

            func(*owner, args...);
        }
    };
} // namespace PJ
