#pragma once

#include "SDLWorld.h"
#include "SomePlatformWindow.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

// CODE REVIEW: ?/23
namespace PJ {
    class SDLPlatformWindow : public SomePlatformWindow {
    public:
        struct Configuration {
            enum class RendererType {
                // Create window only
                Window,

                // Create window and renderer
                WindowAndRenderer
            };

            Vector2Int size;
            uint32_t windowFlags;
            RendererType rendererType;

            Configuration(Vector2Int size, uint32_t windowFlags, RendererType rendererType) :
                size(size),
                windowFlags(windowFlags),
                rendererType(rendererType) {}

            /// Use platform native renderer
            static Configuration const native;

            /// Always use OpenGL renderer
            static Configuration const openGL;

            // Window is fullscreen at the size you set
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

            // Matches the size of the desktop (ignores size)
            void SetIsFullscreenDesktop(bool value) {
                SetFlag(SDL_WINDOW_MAXIMIZED, value);
            }

            // On MacOS, requires NSHighResolutionCapable
            void SetAllowHighDPI(bool value) {
                SetFlag(SDL_WINDOW_HIGH_PIXEL_DENSITY, value);
            }

            void SetIsAlwaysOnTop(bool value) {
                SetFlag(SDL_WINDOW_ALWAYS_ON_TOP, value);
            }

            void SetIsMetal(bool value) {
                SetFlag(SDL_WINDOW_METAL, value);
            }

            // Future: add missing flag funcs if needed

            void SetFlag(uint32_t flag, bool value) {
                if (value) {
                    windowFlags |= flag;
                } else {
                    windowFlags &= ~flag;
                }
            }
        };

    protected:
        SDL_Renderer* renderer = nullptr;

        SDL_Window* window = nullptr;
        SP<SDLWorld> world;
        Configuration config;
        Vector2Int configureSize;

    public:
        using Base = SomePlatformWindow;

        SDLPlatformWindow(Configuration config) :
            config(config) {}

        virtual ~SDLPlatformWindow() {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
        }

        SP<SDLWorld> World() const {
            return world;
        }

        void SetWorld(SP<SDLWorld> world) {
            this->world = world;
        }

        SDL_Window* SDL_Window() const {
            return window;
        }

        SDL_Renderer* SDL_Renderer() const {
            return renderer;
        }

        operator struct SDL_Window *() {
            return window;
        }

        float PixelDensity() const {
            GUARDR(window, 0)
            return SDL_GetWindowPixelDensity(window);
        }

        // MARK: SomePlatformWindow

        Vector2Int Size() const override {
            GUARDR(window, Vector2Int::zero)

            int x, y;
            SDL_GetWindowSize(window, &x, &y);
            return Vector2Int(x, y);
        }

        Vector2Int PixelSize() const override {
            GUARDR(window, Vector2Int::zero)

            int x, y;
            SDL_GetWindowSizeInPixels(window, &x, &y);
            return Vector2Int(x, y);
        }

    protected:
        void GoInternal() override;
    };
} // namespace PJ
