#pragma once

#include "Base.h"
#include "Class.h"
#include "Vector2.h"

/*
 RATING: 4 stars
 Simple protocol
 CODE REVIEW: 10/6/24
 */
namespace PJ {
    class World;

    /// Platform-level window that contains a world and render context for renders
    class SomePlatformWindow : public Base {
    public:
        using This = SomePlatformWindow;

        /// Each window has its own world
        SP<SDLWorld> world;

        SomePlatformWindow(SP<SDLWorld> world) :
            world(world) {}

        virtual Vector2Int Size() const = 0;
        virtual Vector2Int PixelSize() const = 0;
    };
} // namespace PJ
