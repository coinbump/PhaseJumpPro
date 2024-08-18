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
    if (nullptr == window) {
        return;
    }

    // https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html
    // https://cpp.hotexamples.com/examples/-/-/ImGui_ImplSDL2_NewFrame/cpp-imgui_implsdl2_newframe-function-examples.html
    IMGUI_CHECKVERSION();
    imGuiContext = ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // For Mac OS:
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glContext = SDL_GL_CreateContext(window);
    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    SP<GLRenderEngine> glRenderEngine = MAKE<GLRenderEngine>();
    //    glRenderEngine->colorFormat = ColorFormat::Byte;
    glRenderEngine->Go();

    this->renderEngine = SCAST<SomeRenderEngine>(glRenderEngine);
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

void SDLImGuiRenderContext::SetImGuiContext(ImGuiContext* imGuiContext) {
    this->imGuiContext = imGuiContext;
}

void SDLImGuiRenderContext::Bind() {
    if (nullptr == window || nullptr == imGuiContext) {
        return;
    }

    ImGui::SetCurrentContext(imGuiContext);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    auto io = ImGui::GetIO();

    _GLRenderEngine()->SetViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glUseProgram(0); // You may want this if using this code in an OpenGL 3+
                     // context where shaders may be bound
}

void SDLImGuiRenderContext::Clear() {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#include "EllipseMeshBuilder.h"
#include "RenderModel.h"

void SDLImGuiRenderContext::Present() {
    if (nullptr == window) {
        return;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

Vector2 SDLImGuiRenderContext::Size() const {
    auto io = ImGui::GetIO();
    return Vector2(io.DisplaySize.x, io.DisplaySize.y);
}

Vector2Int SDLImGuiRenderContext::PixelSize() const {
    auto io = ImGui::GetIO();
    return Vector2Int(io.DisplaySize.x, io.DisplaySize.y);
}
