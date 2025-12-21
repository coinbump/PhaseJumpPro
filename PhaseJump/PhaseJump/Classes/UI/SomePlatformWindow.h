#pragma once

#include "Base.h"
#include "Class.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    class World;

    /// Platform-level window that contains a world and render context for renders
    class SomePlatformWindow : public Base {
    public:
        using This = SomePlatformWindow;

        SomePlatformWindow() {}

        /// @return Returns the world associated with this window
        virtual SP<World> GetWorld() const = 0;

        /// Sets the world for this window
        virtual void SetWorld(SP<World> value) = 0;

        /// @return Returns window's size in points
        virtual Vector2Int Size() const = 0;

        /// @return Returns windows size in pixels
        virtual Vector2Int PixelSize() const = 0;
    };
} // namespace PJ
