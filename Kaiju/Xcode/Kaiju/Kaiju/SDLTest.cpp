#ifndef _TESTS_

#include "SDLTest.h"
#include "KaijuImGuiRenderProcessor.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3_image/SDL_image.h>
#include <PhaseJump/PhaseJump.h>
#include <iostream>
#include "TestGradientsScene.h"
#include "TestMeshPathScene.h"
#include "TestTextureScene.h"
#include "TestSlicedTextureScene.h"
#include "TestAnimatedTexturesScene.h"

using namespace PJ;
using namespace std;

class TestWorld : public SDLWorld {
public:
    using Base = SDLWorld;

    TestWorld() {
    }

protected:
    void GoInternal() override {
        Base::GoInternal();
    }
};

void SDLFoo() {
//    * SDL_InitSubSystem() might be preferred.
//    *
//    * The file I/O (for example: SDL_IOFromFile) and threading (SDL_CreateThread)
//    * subsystems are initialized by default. Message boxes
//    * (SDL_ShowSimpleMessageBox) also attempt to work without initializing the
//    * video subsystem, in hopes of being useful in showing an error dialog when
//    * SDL_Init fails. You must specifically initialize other subsystems if you
//    * use them in your application.
//    *
//    * Logging (such as SDL_Log) works without initialization, too.
//    *
//    * `flags` may be any of the following OR'd together:
//    *
//    * - `SDL_INIT_TIMER`: timer subsystem
//    * - `SDL_INIT_AUDIO`: audio subsystem; automatically initializes the events
//    *   subsystem
//    * - `SDL_INIT_VIDEO`: video subsystem; automatically initializes the events
//    *   subsystem
//    * - `SDL_INIT_JOYSTICK`: joystick subsystem; automatically initializes the
//    *   events subsystem
//    * - `SDL_INIT_HAPTIC`: haptic (force feedback) subsystem
//    * - `SDL_INIT_GAMEPAD`: gamepad subsystem; automatically initializes the
//    *   joystick subsystem
//    * - `SDL_INIT_EVENTS`: events subsystem
//    * - `SDL_INIT_SENSOR`: sensor subsystem; automatically initializes the events
//    *   subsystem
//    * - `SDL_INIT_CAMERA`: camera subsystem; automatically initializes the events
//    *   subsystem

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);

    /* Enable standard application logging */
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

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

    if (SDL_HasGamepad()) {
        SDL_Gamepad *gamepad;
        SDL_SetGamepadEventsEnabled(SDL_TRUE);
        gamepad = SDL_OpenGamepad(0);
        SDL_SetGamepadLED(gamepad, 255, 0, 0);
    } else {
        SDL_Joystick *joystick;
        SDL_SetJoystickEventsEnabled(SDL_TRUE);
        joystick = SDL_OpenJoystick(0);
    }

    cout << "Is Joystick Enabled?: " << SDL_JoystickEventsEnabled();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    auto windowConfig = SDLPlatformWindow::Configuration::openGL;
    windowConfig.size = Vector2Int(640, 480);
    windowConfig.SetIsResizable(true);
    windowConfig.SetIsFullscreenDesktop(true);
    auto window = MAKE<SDLPlatformWindow>(windowConfig);
    window->SetWorld(MAKE<TestWorld>());
    window->Go();

    auto world = window->World();

    auto renderContext = MAKE<SDLImGuiRenderContext>();
    renderContext->clearColor = Color(.8, .8, .8, 1.0f);
    renderContext->Configure(*window);

    world->SetRenderContext(renderContext);
    world->uiEventPoller = MAKE<SDLImGuiUIEventPoller>(*window);

    SP<LoadResourcesModel> loadResourcesModel = SCAST<LoadResourcesModel>(MAKE<StandardLoadResourcesModel>(StandardLoadResourcesModel::LoadType::Rez));
    SP<FileManager> fm = MAKE<FileManager>();
    ResourceScanner resourceScanner(loadResourcesModel, fm);
    ResourceRepository resourceRepository(loadResourcesModel, world->loadedResources, fm);

    FilePath path = SDL_GetBasePath();
    path /= FilePath("resources/art");

    // TODO: we don't need to scan anymore, with single .rez file
    auto loadResourcesPlan = resourceScanner.ScanAt(path, true);
    auto allInfos = loadResourcesPlan.AllInfos();
    for (auto& info : allInfos) {
        resourceRepository.Load(info);
    }

    auto designSystem = MAKE<DesignSystem>();
    designSystem->Add(UIElement::SliderTrack, world->Texture("slider-track"));
    designSystem->Add(UIElement::SliderThumb, world->Texture("slider-thumb"));
    designSystem->Add(UIElement::SliderVerticalTrack, world->Texture("slider-track-v"));
    designSystem->Add(UIElement::SliderVerticalThumb, world->Texture("slider-thumb-v"));
    designSystem->SetTag(UIElement::SliderTrack, UITag::SlicePoints, SlicedTextureRenderer::SlicePoints{{12, 12}, {12, 12}});
    designSystem->SetTag(UIElement::SliderTrack, UITag::EndCapSize, 20.0f);

    world->designSystem = designSystem;

    // Register a mouse device
    //Mouse::current = 
    MAKE<SDLMouseDevice>();

    auto testTextureNode = MAKE<WorldNode>();
    auto testTextureScene = MAKE<TestTextureScene>();
    testTextureNode->Add(testTextureScene);
    world->Add(testTextureNode);

    auto uiSystem = MAKE<UISystem>();
    uiSystem->typeTags.insert("editor.vip");
    world->Add(uiSystem);

    auto renderSystem = MAKE<RenderWorldSystem>();
    renderSystem->typeTags.insert("editor.vip");
    world->Add(renderSystem);

    auto showMesh = MAKE<ShowMeshRenderProcessor>();
    showMesh->Enable(false);
    renderSystem->Add(showMesh);

    auto cameraCull = MAKE<CameraCullRenderProcessor>();
    renderSystem->Add(cameraCull);

    auto order = MAKE<OrderRenderProcessor>([](RenderModel const& lhs, RenderModel const& rhs) {
        return lhs.mesh.GetBounds().Min().x < rhs.mesh.GetBounds().Min().x;
    });
    order->Enable(false);
    renderSystem->Add(order);

    auto imGuiProcessor = MAKE<KaijuImGuiRenderProcessor>(*world);
    renderSystem->Add(imGuiProcessor);
    
    auto depthFirstOrder = MAKE<DepthFirstOrderRenderProcessor>();
    depthFirstOrder->Enable(false);
    renderSystem->Add(depthFirstOrder);

    auto showColliders = MAKE<ShowCollidersRenderProcessor>();
    showColliders->Enable(false);
    renderSystem->Add(showColliders);
//    renderSystem->AddProcessor<ShowCollidersRenderProcessor>()
//        .SetActive(false)

    auto showBounds = MAKE<ShowBoundsRenderProcessor>();
    showBounds->Enable(false);
    renderSystem->Add(showBounds);

    auto overrideFeatures = MAKE<OverrideFeaturesRenderProcessor>();
    overrideFeatures->map[RenderFeature::Blend] = RenderFeatureState::Disable;
    overrideFeatures->Enable(false);
    renderSystem->Add(overrideFeatures);

    auto batch = MAKE<BatchByMaterialRenderProcessor>();
    renderSystem->Add(batch);

    int count = 0;

//    SDL_PenID* result = SDL_GetPens(&count);

    world->Go();
    world->Run();
}

#endif
