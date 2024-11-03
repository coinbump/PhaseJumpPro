#pragma once

#include "Vector2.h"
#include <SDL3/SDL.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    /// Configuration model for a platform window
    class PlatformWindowConfig {
    public:
        String title;
        Vector2Int size;

        PlatformWindowConfig(String title = {}, Vector2Int size = {}) :
            title(title),
            size(size) {}
    };

    /// Configuration model for an SDL platform window
    class SDLPlatformWindowConfig : public PlatformWindowConfig {
    public:
        using This = SDLPlatformWindowConfig;

        enum class RendererType {
            /// Create window only
            Window,

            /// Create window and renderer
            WindowAndRenderer
        };

        /// Properties for designated initializer
        struct Config {
            String title;
            Vector2Int size;
            uint32_t windowFlags{};
            RendererType rendererType{};
        };

        uint32_t windowFlags{};
        RendererType rendererType{};

        SDLPlatformWindowConfig(
            String title = {}, Vector2Int size = {}, uint32_t windowFlags = {},
            RendererType rendererType = {}
        ) :
            PlatformWindowConfig(title, size),
            windowFlags(windowFlags),
            rendererType(rendererType) {}

        explicit SDLPlatformWindowConfig(Config const& config) :
            SDLPlatformWindowConfig(
                config.title, config.size, config.windowFlags, config.rendererType
            ) {}

        /// Use platform native renderer
        // FUTURE: static Config const native;

        /// Always use OpenGL renderer
        static This const openGL;

        void SetIsFullscreen(bool value) {
            SetFlag(SDL_WINDOW_FULLSCREEN, value);
        }

        void SetIsOpenGL(bool value) {
            SetFlag(SDL_WINDOW_OPENGL, value);
        }

        void SetIsBorderless(bool value) {
            SetFlag(SDL_WINDOW_BORDERLESS, value);
        }

        void SetIsResizable(bool value) {
            SetFlag(SDL_WINDOW_RESIZABLE, value);
        }

        void SetIsMinimized(bool value) {
            SetFlag(SDL_WINDOW_MINIMIZED, value);
        }

        void SetIsMaximized(bool value) {
            SetFlag(SDL_WINDOW_MAXIMIZED, value);
        }

        void SetIsFullscreenDesktop(bool value) {
            SetFlag(SDL_WINDOW_MAXIMIZED, value);
        }

        void SetAllowHighDPI(bool value) {
            SetFlag(SDL_WINDOW_HIGH_PIXEL_DENSITY, value);
        }

        void SetIsAlwaysOnTop(bool value) {
            SetFlag(SDL_WINDOW_ALWAYS_ON_TOP, value);
        }

        void SetIsMetal(bool value) {
            SetFlag(SDL_WINDOW_METAL, value);
        }

        // FUTURE: add missing flags as needed

        void SetFlag(uint32_t flag, bool value) {
            if (value) {
                windowFlags |= flag;
            } else {
                windowFlags &= ~flag;
            }
        }
    };
} // namespace PJ
