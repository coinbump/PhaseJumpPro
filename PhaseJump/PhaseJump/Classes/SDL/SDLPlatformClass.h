#pragma once

#include "SDLPlatformConfig.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    namespace ConfigId {
        auto constexpr GameEditorGL = "game.editor.gl";
        auto constexpr GameReleaseGL = "game.release.gl";
    } // namespace ConfigId

    /// Result returned after configuring SDL
    class SDLConfigPlatformResult {
    public:
        /// Platform window that wraps the SDL window
        SP<SDLPlatformWindow> window;

        SDLConfigPlatformResult(SP<SDLPlatformWindow> window) :
            window(window) {}
    };

    /// Stores standard configs for quick setup of SDL based on the type of app we're building
    class SDLPlatformClass {
    public:
        UnorderedMap<String, UP<SDLPlatformConfig>> configs;

        SDLPlatformClass();

        SDLConfigPlatformResult Configure(SDLPlatformConfig& config, SP<SDLWorld> world);
    };
} // namespace PJ
