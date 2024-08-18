// #pragma once
//
// #include "Color.h"
// #include "SomeRenderContext.h"
// #include <SDL3/SDL.h>
//
// class ImGuiIO;
// class ImGuiContext;
//
//// TODO: this is a WORK IN PROGRESS
// namespace PJ {
//     class SDLPlatformWindow;
//     class SomeGLRenderEngine;
//
//     /// Renders `ImGUI` widgets via `SDL`
//     class SDLGLRenderContext : public SomeRenderContext {
//     protected:
//         SDL_Window* window = nullptr;
//         SDL_GLContext glContext = NULL;
//         ImGuiContext* imGuiContext = nullptr;
//
//     public:
//         Color clearColor = Color::gray;
//
//         SDLGLRenderContext() {}
//
//         virtual ~SDLGLRenderContext();
//
//         void Configure(SDL_Window* window);
//
//         // TODO: SP-audit
//         SP<SomeGLRenderEngine> _GLRenderEngine() const;
//
//         // Avoid these (use Configure instead)
//         void SetWindow(SDL_Window* window);
//         void SetGLContext(SDL_GLContext glContext);
//         void SetImGuiContext(ImGuiContext* imGuiContext);
//
//         // MARK: - SomeRenderContext
//
//         void Bind() override;
//         void Clear() override;
//         void Present() override;
//         Vector2 Size() const override;
//         Vector2Int PixelSize() const override;
//     };
// } // namespace PJ
