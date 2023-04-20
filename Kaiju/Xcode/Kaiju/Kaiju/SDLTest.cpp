#ifndef _TESTS_

#include "SDLTest.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2_image/SDL_image.h>
#include <PhaseJump/PhaseJump.h>
#include <iostream>
#include "TestColorVaryScene.h"
#include "TestMeshPathScene.h"
#include "TestTextureScene.h"

using namespace PJ;
using namespace std;

bool my_tool_active = false;
float my_color[4];
bool isDone = false;
bool show_demo_window = true;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

class SDLImGuiEventPoller : public SomeUIEventPoller {
public:
    SDL_Window* window;

    SDLImGuiEventPoller(SDL_Window* window) : window(window) {
    }

    Status PollUIEvents() override {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT
                || (event.type == SDL_WINDOWEVENT
                    && event.window.event == SDL_WINDOWEVENT_CLOSE
                 && event.window.windowID == SDL_GetWindowID(window))) {
                return Status::Done;
            }
        }

        return Status::Running;
    }
};

//#define IMG

std::shared_ptr<GLTexture> texture;

void SDLFoo() {
    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

//    int value;
//    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
//    PJLog("SDL_GL_DOUBLEBUFFER: %d", value);
//    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
//    PJLog("SDL_GL_DEPTH_SIZE: %d", value);
//    SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &value);
//    PJLog("SDL_GL_STENCIL_SIZE: %d", value);
//    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
//    PJLog("SDL_GL_CONTEXT_MAJOR_VERSION: %d", value);
//    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
//    PJLog("SDL_GL_CONTEXT_MINOR_VERSION: %d", value);

//    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    auto windowConfig = SDLWindow::Configuration::openGL;
    windowConfig.SetIsResizable(true);
    windowConfig.SetIsFullscreenDesktop(true);
    auto window = std::make_shared<SDLWindow>(windowConfig, Vector2Int(640, 480));
    window->Go();

    auto renderContext = std::make_shared<SDLImGuiRenderContext>();
    renderContext->clearColor = Color(0.2f, 0.8f, 0.1f, 1.0f);
    renderContext->Configure(*window);

    auto node = std::make_shared<WorldNode>();
    auto component = std::make_shared<FuncRenderer>([] (RenderIntoModel renderIntoModel) { ImGui::ShowDemoWindow(&show_demo_window); });
    node->AddComponent(component);
    window->World()->Add(node);
    window->World()->SetRenderContext(renderContext);
    window->World()->uiEventPoller = std::make_shared<SDLImGuiEventPoller>(*window);

    TestMeshPathScene testMeshPathScene;
    testMeshPathScene.LoadInto(*window->World());

    TestColorVaryScene testColorVaryScene;
    testColorVaryScene.LoadInto(*window->World());

    TestTextureScene testTextureScene;
    testTextureScene.LoadInto(*window->World());

    FilePath path = SDL_GetBasePath();
    path /= FilePath("resources/heart-full.png");

    auto loadTexture = std::make_shared<SDLLoadGLTextureOperation>(path, TextureMagnification::Nearest);
    loadTexture->Go();
    texture = loadTexture->texture;

    window->World()->Go();
}

#endif
