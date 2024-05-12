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
#include "TestSlicedSpriteScene.h"
#include "TestAnimatedTexturesScene.h"

using namespace PJ;
using namespace std;

bool my_tool_active = false;
float my_color[4];
bool isDone = false;
bool show_demo_window = true;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//#define IMG

SP<GLTexture> texture;

class TestWorld : public SDLWorld {
public:
    using Base = SDLWorld;

    TestWorld() {
    }

protected:
    void GoInternal() override {
        Base::GoInternal();

        auto cameraNode = Camera::main->owner.lock();
        auto raycaster = std::make_shared<SimpleRaycaster2D>();
        cameraNode->AddComponent(raycaster);
    }

    void ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) override {
        Base::ProcessUIEvents(uiEvents);

        for (auto event : uiEvents) {
            auto dropFileEvent = DCAST<DropFilesUIEvent>(event);
            if (dropFileEvent) {
                PJLog("Drop File");
                root->Clear();

                auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());

                auto cameraNode = MAKE<WorldNode>();
                cameraNode->Add(camera);
                root->AddEdge(StandardEdgeModel(), cameraNode);

                VectorList<SP<SomeTexture>> glTextures;
                for (auto filePath : dropFileEvent->filePaths) {
                    auto loadTexture = MAKE<SDLLoadGLTextureOperation>(filePath, TextureMagnification::Linear);
                    loadTexture->Run();
                    auto textures = loadTexture->result->SuccessValue();
                    if (textures.size() > 0) {
                        texture = SCAST<GLTexture>(textures[0].resource);
                        glTextures.Add(texture);
                    }
                }

                TestAnimatedTexturesScene::TextureList textures2{glTextures};
                TestAnimatedTexturesScene scene(textures2);
                scene.LoadInto(*this);
                continue;
            }

            auto pointerDownEvent = DCAST<PointerDownUIEvent<ScreenPosition>>(event);
            if (pointerDownEvent) {
                cout << "Pointer down at: " << pointerDownEvent->pressPosition.x << ", " << pointerDownEvent->pressPosition.y << "\n";
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
    auto window = MAKE<SDLWindow>(windowConfig, Vector2Int(640, 480));
    window->SetWorld(MAKE<TestWorld>());
    window->Go();

    auto eventSystem = MAKE<UISystem>();
    auto systemNode = MAKE<WorldNode>();
    systemNode->AddComponent(eventSystem);
    window->World()->root->AddChild(systemNode);

    auto renderContext = MAKE<SDLImGuiRenderContext>();
    renderContext->clearColor = Color(0.2f, 0.8f, 0.1f, 1.0f);
    renderContext->Configure(*window);

    auto node = MAKE<WorldNode>();
    auto component = MAKE<FuncRenderer>([] (RenderIntoModel renderIntoModel) { ImGui::ShowDemoWindow(&show_demo_window); });
    node->AddComponent(component);
    window->World()->Add(node);
    window->World()->SetRenderContext(renderContext);
    window->World()->uiEventPoller = MAKE<SDLImGuiUIEventPoller>(*window);

    TestMeshPathScene testMeshPathScene;
    testMeshPathScene.LoadInto(*window->World());

//    TestColorVaryScene testColorVaryScene;
//    testColorVaryScene.LoadInto(*window->World());

    SP<SomeLoadResourcesModel> loadResourcesModel = SCAST<SomeLoadResourcesModel>(MAKE<StandardLoadResourcesModel>());
    SP<FileManager> fm = MAKE<FileManager>();
    ResourceRepository resourceRepository(loadResourcesModel, window->World()->loadedResources, fm);

    FilePath path = SDL_GetBasePath();
    path /= FilePath("resources/art");

    auto loadResourcesPlan = resourceRepository.Scan(path, true);
    auto allInfos = loadResourcesPlan.AllInfos();
    for (auto info : allInfos) {
        resourceRepository.Load(info);
    }

    // Register a mouse device
    Mouse::current = MAKE<SDLMouseDevice>();

    texture = DCAST<GLTexture>(window->World()->loadedResources->map["texture"]["heart-full"].resource);
    TestTextureScene testTextureScene(texture);
    testTextureScene.LoadInto(*window->World());

    auto sliceTexture = DCAST<GLTexture>(window->World()->loadedResources->map["texture"]["example-button-normal"].resource);
    TestSlicedSpriteScene testSlicedSpriteScene(sliceTexture);
//    testSlicedSpriteScene.LoadInto(*window->World());

    window->World()->Go();
    window->World()->Run();
}

#endif
