#pragma once

#include "DevProfiler.h"
#include "PlaneMeshBuilder.h"
#include "SDLTextureRenderer.h"
#include "SomeMeshBuilder.h"
#include "SomeRenderer.h"
#include "VectorList.h"
#include "World.h"
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

// #define PROFILE

// CODE REVIEW: ?/23
namespace PJ {
    /// A world with a SDL main loop
    class SDLWorld : public World {
    public:
        using Base = World;
        using This = SDLWorld;

    protected:
        /// Becomes true when the app is finished polling events
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

        virtual void Configure(SDL_Window* window, SP<SomeRenderContext> renderContext) {
            this->window = window;
            this->renderContext = renderContext;
        }

    protected:
        void MainLoop();

        // MARK: World

        void GoInternal() override;
    };
} // namespace PJ
