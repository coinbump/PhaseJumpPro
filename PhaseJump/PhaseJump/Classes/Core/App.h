#pragma once

#include "QuickBuilder.h"
#include "SDLWorld.h"

// TODO: REVIEW. Don't use inheritance(?)
namespace PJ {
    /**
     Exends QuickBuilder for building new apps

     Usage: App::New().Circle(100, Color::red).Go();
     */
    class AppBuilder : public QuickBuilder {
    public:
        using Base = QuickBuilder;
        using This = AppBuilder;

        AppBuilder(WorldNode& node) :
            Base(node) {}

    protected:
        // MARK: Base

        void OnGo() override;
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
