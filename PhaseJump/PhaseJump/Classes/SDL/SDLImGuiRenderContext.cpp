#include "SDLImGuiRenderContext.h"
#include "Color.h"
#include "GLHeaders.h"
#include "GLRenderEngine.h"
#include "GLShaderProgram.h"
#include "GLShaderProgramLoader.h"
#include "GLVertexBuffer.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "SDLPlatformWindow.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

using namespace std;
using namespace PJ;

SDLImGuiRenderContext::~SDLImGuiRenderContext() {
    if (glContext) {
        SDL_GL_DestroyContext(glContext);
    }

    if (imGuiContext) {
        ImGui::SetCurrentContext(imGuiContext);
        ImGui::DestroyContext();
    }
}

void SDLImGuiRenderContext::Configure(SDL_Window* window) {
    this->window = window;
    GUARD(window)

    // For Mac OS:
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
    ImGui_ImplOpenGL3_Init(); // ALT: "#version 150");
}

SP<SomeGLRenderEngine> SDLImGuiRenderContext::_GLRenderEngine() const {
    return SCAST<SomeGLRenderEngine>(renderEngine);
}

void SDLImGuiRenderContext::SetWindow(SDL_Window* window) {
    this->window = window;
}

void SDLImGuiRenderContext::SetGLContext(SDL_GLContext glContext) {
    this->glContext = glContext;
}

void SDLImGuiRenderContext::SetImGuiContext(class ImGuiContext* imGuiContext) {
    this->imGuiContext = imGuiContext;
}

void SDLImGuiRenderContext::Bind() {
    GUARD(window)

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto pixelSize = PixelSize();

    // TODO: shouldn't be relying on GL here
    _GLRenderEngine()->SetViewport(0, 0, pixelSize.x, pixelSize.y);
}

void SDLImGuiRenderContext::Clear() {
    // TODO: shouldn't be relying on GL here
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SDLImGuiRenderContext::Present() {
    GUARD(window)

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    SDL_GL_SwapWindow(window);
}

Vector2 SDLImGuiRenderContext::Size() const {
    GUARDR(window, vec2Zero)

    int x, y;
    SDL_GetWindowSize(window, &x, &y);
    return Vector2(x, y);
}

Vector2Int SDLImGuiRenderContext::PixelSize() const {
    GUARDR(window, Vector2Int(0, 0))

    int x, y;
    SDL_GetWindowSizeInPixels(window, &x, &y);
    return Vector2Int(x, y);
}
