#ifndef PJSDLIMGUIRENDERCONTEXT_H
#define PJSDLIMGUIRENDERCONTEXT_H

#include "Color.h"
#include "SomeRenderContext.h"
#include <SDL3/SDL.h>

class ImGuiIO;
class ImGuiContext;

// CODE REVIEW: ?/23
namespace PJ {
    class SDLPlatformWindow;
    class SomeGLRenderEngine;

    /// Renders `ImGUI` widgets via `SDL`
    class SDLImGuiRenderContext : public SomeRenderContext {
    protected:
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = NULL;
        ImGuiContext* imGuiContext = nullptr;

    public:
        Color clearColor = Color::gray;

        SDLImGuiRenderContext() {}

        virtual ~SDLImGuiRenderContext();

        void Configure(SDL_Window* window);

        SP<SomeGLRenderEngine> _GLRenderEngine() const;

        // Avoid these (use Configure instead)
        void SetWindow(SDL_Window* window);
        void SetGLContext(SDL_GLContext glContext);
        void SetImGuiContext(ImGuiContext* imGuiContext);

        // MARK: - SomeRenderContext

        void Bind() override;
        void Clear() override;
        void Present() override;
        Vector2 Size() const override;
        Vector2Int PixelSize() const override;
    };
} // namespace PJ

#endif
