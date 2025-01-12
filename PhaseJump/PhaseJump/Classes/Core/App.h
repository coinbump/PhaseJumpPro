#pragma once

#include "QuickBuild.h"
#include "SDLWorld.h"

// REVIEW: /td
namespace PJ {
    /**
     Provides access to a quick builder object when the app is created

     Usage: App::New().qb.Circle(100, Color::red);
     */
    class AppBuilder {
    public:
        World* world{};

        QuickBuild& qb;
        UP<QuickBuild> _qb;

        ~AppBuilder();
    };

    /**
     Convenience for quickly building an app with default configuration
     Similar to Bevy's convenience initializer
     */
    class App {
    public:
        using World = SDLWorld;

        /// Stores the world for a default single window app
        static SP<World> world;

        /// Called by App::New
        static std::function<void()> newFunc;

        /// Creates a new empty app, window, and world
        static AppBuilder New();
    };
} // namespace PJ
