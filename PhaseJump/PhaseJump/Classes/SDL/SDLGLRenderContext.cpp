#include "SDLGLRenderContext.h"
#include "GLHeaders.h"
#include "GLRenderEngine.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "SDLPlatformWindow.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

using namespace std;
using namespace PJ;

SDLGLRenderContext::~SDLGLRenderContext() {
    if (glContext) {
        SDL_GL_DestroyContext(glContext);
    }

    if (imGuiContext) {
        ImGui::SetCurrentContext(imGuiContext);
        ImGui::DestroyContext();
    }
}

void SDLGLRenderContext::Configure(SDL_Window* window) {
    GUARD(nullptr == this->window)

    this->window = window;
    GUARD(window)

    // Disable deprecated OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    glContext = SDL_GL_CreateContext(window);

    // https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html
    // https://cpp.hotexamples.com/examples/-/-/ImGui_ImplSDL2_NewFrame/cpp-imgui_implsdl2_newframe-function-examples.html
    IMGUI_CHECKVERSION();
    imGuiContext = ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init();
}

SomeGLRenderEngine* SDLGLRenderContext::GetGLRenderEngine() const {
    return static_cast<SomeGLRenderEngine*>(&renderEngine);
}

void SDLGLRenderContext::Bind() {
    GUARD(window)

    auto pixelSize = PixelSize();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GetGLRenderEngine()->SetViewport(0, 0, pixelSize.x, pixelSize.y);
}

void SDLGLRenderContext::Clear() {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void SDLGLRenderContext::Present() {
    GUARD(window)

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    SDL_GL_SwapWindow(window);
}

Vector2 SDLGLRenderContext::Size() const {
    GUARDR(window, {})

    int x, y;
    SDL_GetWindowSize(window, &x, &y);
    return Vector2(x, y);
}

Vector2Int SDLGLRenderContext::PixelSize() const {
    GUARDR(window, {})

    int x, y;
    SDL_GetWindowSizeInPixels(window, &x, &y);
    return Vector2Int(x, y);
}
