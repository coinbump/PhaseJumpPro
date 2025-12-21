#pragma once

#include "DevProfiler.h"
#include "PlaneMeshBuilder.h"
#include "Renderer.h"
#include "SDLTextureRenderer.h"
#include "SomeMeshBuilder.h"
#include "VectorList.h"
#include "World.h"
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

/// Enable to enable profiling code called on launch
// #define LAUNCH_PROFILE

/// Enable to enable profiling code
// #define PROFILE

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /**
     A world with a SDL main loop

     Call Configure first, then Go.
     */
    class SDLWorld : public World {
    public:
        using Base = World;
        using This = SDLWorld;

    protected:
        /// True when the app is finished polling events
        bool isFinished{};

        /// SDL window parent for this world (window owns the world)
        SDL_Window* window{};

        /// Start time value for main loop time delta calculation
        uint64_t startTime{};

    public:
        /// Max update time delta value
        /// Prevents problems if the app hitches or is paused by the debugger
        float maxTimeDelta = 1.0f / 30.0f;

        SDLWorld() {}

        SDL_Window* SDLWindow() const {
            return window;
        }

        /// Called before Go to associate types
        virtual void Configure(SDL_Window* window, UP<SomeRenderContext> renderContext) {
            this->window = window;
            this->renderContext = std::move(renderContext);
        }

    protected:
        void MainLoop();

        // MARK: Base

        void OnGo() override;
    };
} // namespace PJ
