#ifndef PJSDLWINDOW_H
#define PJSDLWINDOW_H

#include "SomeWindow.h"
#include "SDLWorld.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace PJ
{
    class SDLWindow : public SomeWindow {
    public:
        struct Configuration {
            enum class RendererType {
                // Create window only
                Window,

                // Create window and renderer
                WindowAndRenderer
            };

            uint32_t windowFlags;
            RendererType rendererType;

            Configuration(uint32_t windowFlags, RendererType rendererType) : windowFlags(windowFlags), rendererType(rendererType) {
            }

            /// Use platform native renderer
            static Configuration const native;

            /// Always use OpenGL renderer
            static Configuration const openGL;

            // Window is fullscreen at the size you set
            void SetIsFullscreen(bool value) { SetFlag(SDL_WINDOW_FULLSCREEN, value); }
            void SetIsOpenGL(bool value) { SetFlag(SDL_WINDOW_OPENGL, value); }
            void SetIsBorderless(bool value) { SetFlag(SDL_WINDOW_BORDERLESS, value); }
            void SetIsResizable(bool value) { SetFlag(SDL_WINDOW_RESIZABLE, value); }
            void SetIsMinimized(bool value) { SetFlag(SDL_WINDOW_MINIMIZED, value); }
            void SetIsMaximized(bool value) { SetFlag(SDL_WINDOW_MAXIMIZED, value); }

            // Matches the size of the desktop (ignores size)
            void SetIsFullscreenDesktop(bool value) { SetFlag(SDL_WINDOW_FULLSCREEN_DESKTOP, value); }

            // On MacOS, requires NSHighResolutionCapable
            void SetAllowHighDPI(bool value) { SetFlag(SDL_WINDOW_ALLOW_HIGHDPI, value); }

            void SetIsAlwaysOnTop(bool value) { SetFlag(SDL_WINDOW_ALWAYS_ON_TOP, value); }
            void SetIsMetal(bool value) { SetFlag(SDL_WINDOW_METAL, value); }

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
        Configuration configuration;

    public:
        using Base = SomeWindow;

        SDLWindow(Configuration configuration, Vector2Int size) : Base(size), configuration(configuration)
        {
        }

        virtual ~SDLWindow() {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
        }

        SP<SDLWorld> World() const { return world; }
        void SetWorld(SP<SDLWorld> world) { this->world = world; }
        
        SDL_Window* SDL_Window() const { return window; }
        SDL_Renderer* SDL_Renderer() const { return renderer; }

        operator struct SDL_Window*() {
            return window;
        }

    protected:
        void GoInternal() override;
    };
}

#endif
