#pragma once

#include "Color.h"
#include "imgui.h"
#include "SomeRenderContext.h"
#include <SDL3/SDL.h>

class ImGuiIO;
class ImGuiContext;

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    class SDLPlatformWindow;
    class BaseGLRenderEngine;

    /// SDL render context that uses OpenGL and has built-in support for imGui
    class SDLGLRenderContext : public RenderContext {
    protected:
        SDL_Window* window{};
        SDL_GLContext glContext{};
        ImGuiContext* imGuiContext{};

    public:
        using Base = RenderContext;
        using This = SDLGLRenderContext;

        SDLGLRenderContext(SomeRenderEngine& renderEngine) :
            Base(renderEngine) {}

        virtual ~SDLGLRenderContext();

        void Configure(SDL_Window* window);

        BaseGLRenderEngine* GetGLRenderEngine() const;

        ImGuiContext* ImGuiContext() const {
            return imGuiContext;
        }

        // MARK: - SomeRenderContext

        void Bind() override;
        void Clear() override;
        void Present() override;
        Vector2 Size() const override;
        Vector2Int PixelSize() const override;
    };
} // namespace PJ
