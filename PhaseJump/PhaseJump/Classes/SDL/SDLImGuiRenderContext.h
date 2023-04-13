#ifndef PJSDLIMGUIRENDERCONTEXT_H
#define PJSDLIMGUIRENDERCONTEXT_H

#include "Color.h"
#include "SomeRenderContext.h"
#include <SDL2/SDL.h>

class ImGuiIO;
class ImGuiContext;

namespace PJ
{
    class SDLWindow;

    /// Renders `ImGUI` widgets via `SDL`
    class SDLImGuiRenderContext : public SomeRenderContext
    {
    protected:
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = NULL;
        ImGuiContext* imGuiContext = nullptr;

    public:
        Color clearColor = Color::gray;

        SDLImGuiRenderContext()
        {
        }

        virtual ~SDLImGuiRenderContext();

        void Configure(SDL_Window* window);

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
}

#endif
