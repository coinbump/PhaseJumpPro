#pragma once

#include "Color.h"
#include "FilePath.h"
#include "SDLPlatformWindowConfig.h"
#include <SDL3/SDL.h>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class SDLWorld;
    class SDLPlatformWindow;

    /// Configuration model for starting a platform SDK
    class PlatformConfig {
    public:
        Color clearColor = Color::white;

        /// (Optional). Render rate in frames per second
        std::optional<float> renderRate;
    };

    /// Configuration model for starting SDL
    class SDLPlatformConfig : public PlatformConfig {
    public:
        /// Flags to initialize SDL
        SDL_InitFlags initFlags;

        /// Config to build SDL platform window
        SDLPlatformWindowConfig windowConfig;

        /// Path to scan for .rez file and load initial resources
        /// Careful: this is only meant for quick prototyping. It will block the main thread
        FilePath resourcesPath;
    };
} // namespace PJ
