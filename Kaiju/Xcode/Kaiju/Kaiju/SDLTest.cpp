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
#include <PhaseJump/PhaseJump.h>
#include <iostream>

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

#define IMG

void SDLFoo() {
    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

#ifdef IMG
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
    renderContext->Configure(*window);

    auto node = std::make_shared<WorldNode>();
    auto component = std::make_shared<FuncRenderer>([] (RenderIntoModel renderIntoModel) { ImGui::ShowDemoWindow(&show_demo_window); });
    node->AddComponent(component);
    window->World()->root->AddEdge(StandardEdgeModel(), node);
    window->World()->SetRenderContext(renderContext);
    window->World()->uiEventPoller = std::make_shared<SDLImGuiEventPoller>(*window);
    window->World()->Go();
#else
    auto windowConfig = SDLWindow::Configuration::native;
    windowConfig.SetIsResizable(true);
    windowConfig.SetIsFullscreenDesktop(true);
    auto window = std::make_shared<SDLWindow>(windowConfig, Vector2Int(640, 480));
    window->Go();

    static std::shared_ptr<SDLTexture> sprite;
    if (!sprite) {
        SDLLoadTextureOperation loadTexture(window->SDL_Renderer(), "resources/heart-full.png");
        loadTexture.Go();
        sprite = loadTexture.texture;
    }

    if (!sprite) {
        std::cout << "ERROR: Missing sprite.";
        return;
    }

    auto pathNode = std::make_shared<WorldNode>();
    auto pathComponent = std::make_shared<CirclePathLayout2D>(100);
    pathNode->AddComponent(pathComponent);
    window->World()->root->AddEdge(StandardEdgeModel(), pathNode);

    for (int i = 0; i < 20; i++) {
        auto node = std::make_shared<WorldNode>();
        pathNode->AddEdge(StandardEdgeModel(), node);
        auto renderer = std::make_shared<SDLTextureRenderer>(sprite);
        node->Add(renderer);
        node->transform->position = Vector3(StandardRandom().Value() * 100.0f, StandardRandom().Value() * 100.0f, 0);
    }

    pathComponent->ApplyLayout();

    window->World()->Go();
#endif
}

#endif
