#pragma once

#include "Base.h"
#include "Class.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class World;
    class SomeRenderEngine;

    /// Platform-level window that contains a world and render context for renders
    class SomePlatformWindow : public Base {
    protected:
        /// ECS nodes, systems, and components
        SP<World> world;

    public:
        using This = SomePlatformWindow;

        SomePlatformWindow() {}

        SP<World> GetWorld() const {
            return world;
        }

        void SetWorld(SP<World> value);

        /// @return Returns window's size in points
        virtual Vector2Int Size() const = 0;

        /// @return Returns windows size in pixels
        virtual Vector2Int PixelSize() const = 0;
    };
} // namespace PJ
