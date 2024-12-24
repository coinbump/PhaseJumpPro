#pragma once

#include "Base.h"
#include "Class.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class World;
    class SomeRenderEngine;

    /// Platform-level window that contains a world and render context for renders
    class SomePlatformWindow : public Base {
    public:
        using This = SomePlatformWindow;

        /// ECS nodes, systems, and components
        SP<World> world;

        /// Renders the world
        SP<SomeRenderEngine> renderEngine;

        SomePlatformWindow() {}

        SP<World> GetWorld() const {
            return world;
        }

        void SetWorld(SP<World> world) {
            this->world = world;
        }

        /// @return Returns window's size in points
        virtual Vector2Int Size() const = 0;

        /// @return Returns windows size in pixels
        virtual Vector2Int PixelSize() const = 0;
    };
} // namespace PJ
