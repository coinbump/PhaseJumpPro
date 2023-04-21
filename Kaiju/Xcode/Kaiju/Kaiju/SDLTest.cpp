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

//#define IMG

std::shared_ptr<GLTexture> texture;

class TestWorld : public SDLWorld {
public:
    using Base = SDLWorld;

    TestWorld() {
    }

protected:
    void ProcessUIEvents(VectorList<std::shared_ptr<SomeUIEvent>> const& uiEvents) override {
        SDLWorld::ProcessUIEvents(uiEvents);

        for (auto event : uiEvents) {
            auto dropFileEvent = dynamic_pointer_cast<DropFileUIEvent>(event);
            if (dropFileEvent) {
                root->Clear();

                auto camera = std::static_pointer_cast<SomeCamera>(std::make_shared<OrthoCamera>());

                auto cameraNode = std::make_shared<WorldNode>();
                cameraNode->Add(camera);
                root->AddEdge(StandardEdgeModel(), cameraNode);

                auto loadTexture = std::make_shared<SDLLoadGLTextureOperation>(dropFileEvent->filePath, TextureMagnification::Linear);
                loadTexture->Go();
                texture = loadTexture->texture;

                TestTextureScene testTextureScene(texture);
                testTextureScene.LoadInto(*this);
                continue;
            }
        }
    }
};

void SDLFoo() {
    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    int value;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
    PJLog("SDL_GL_DOUBLEBUFFER: %d", value);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
    PJLog("SDL_GL_DEPTH_SIZE: %d", value);
    SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &value);
    PJLog("SDL_GL_STENCIL_SIZE: %d", value);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    PJLog("SDL_GL_CONTEXT_MAJOR_VERSION: %d", value);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    PJLog("SDL_GL_CONTEXT_MINOR_VERSION: %d", value);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    auto windowConfig = SDLWindow::Configuration::openGL;
    windowConfig.SetIsResizable(true);
    windowConfig.SetIsFullscreenDesktop(true);
    auto window = std::make_shared<SDLWindow>(windowConfig, Vector2Int(640, 480));
    window->SetWorld(std::make_shared<TestWorld>());
    window->Go();

    auto renderContext = std::make_shared<SDLImGuiRenderContext>();
    renderContext->clearColor = Color(0.2f, 0.8f, 0.1f, 1.0f);
    renderContext->Configure(*window);

    auto node = std::make_shared<WorldNode>();
    auto component = std::make_shared<FuncRenderer>([] (RenderIntoModel renderIntoModel) { ImGui::ShowDemoWindow(&show_demo_window); });
    node->AddComponent(component);
    window->World()->Add(node);
    window->World()->SetRenderContext(renderContext);
    window->World()->uiEventPoller = std::make_shared<SDLImGuiUIEventPoller>(*window);

    TestMeshPathScene testMeshPathScene;
    testMeshPathScene.LoadInto(*window->World());

//    TestColorVaryScene testColorVaryScene;
//    testColorVaryScene.LoadInto(*window->World());

    FilePath path = SDL_GetBasePath();
    path /= FilePath("resources/heart-full.png");

    auto loadTexture = std::make_shared<SDLLoadGLTextureOperation>(path, TextureMagnification::Nearest);
    loadTexture->Go();
    texture = loadTexture->texture;

    TestTextureScene testTextureScene(texture);
    testTextureScene.LoadInto(*window->World());

    window->World()->Go();
}

#endif
