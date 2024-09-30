#ifndef _TESTS_

#include "SDLTest.h"
#include "KaijuImGuiRenderProcessor.h"
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif
#include "TestAnimateApp.h"
#include "TestGradientsScene.h"
#include "TestMeshPathScene.h"
#include "TestSlicedTextureScene.h"
#include "TestTextureScene.h"
#include <iostream>

using namespace PJ;
using namespace std;

class KaijuWorld : public SDLWorld {
public:
    using Base = SDLWorld;

    KaijuWorld() {}

protected:
    void GoInternal() override {
        Base::GoInternal();
    }
};

void KaijuGo() {
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);

    /* Enable standard application logging */
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    //    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //
    //    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    auto windowConfig = SDLPlatformWindow::Configuration::openGL;
    windowConfig.size = Vector2Int(1920, 1080);
    windowConfig.SetIsResizable(true);
    windowConfig.SetIsFullscreenDesktop(true);
    auto world = MAKE<KaijuWorld>();
    auto window = MAKE<SDLPlatformWindow>(windowConfig, world);
    window->Go();

    if (SDL_HasGamepad()) {
        SDL_Gamepad* gamepad;
        SDL_SetGamepadEventsEnabled(SDL_TRUE);
        gamepad = SDL_OpenGamepad(0);
        SDL_SetGamepadLED(gamepad, 255, 0, 0);
    } else {
        SDL_Joystick* joystick;
        SDL_SetJoystickEventsEnabled(SDL_TRUE);
        joystick = SDL_OpenJoystick(0);
    }

    cout << "Is Joystick Enabled?: " << SDL_JoystickEventsEnabled();

    SP<GLRenderEngine> glRenderEngine = MAKE<GLRenderEngine>();
    glRenderEngine->colorFormat = ColorFormat::Byte;

    auto renderContext = MAKE<SDLImGuiRenderContext>(glRenderEngine);
    renderContext->clearColor = Color(.8, .8, .8, 1.0f);
    renderContext->Configure(*window);

    /// Must create context before render engine can go
    glRenderEngine->Go();

    world->Configure(window->SDL_Window(), renderContext);

    int value;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
    PJ::Log("SDL_GL_DOUBLEBUFFER: ", value);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
    PJ::Log("SDL_GL_DEPTH_SIZE: ", value);
    SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &value);
    PJ::Log("SDL_GL_STENCIL_SIZE: ", value);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    PJ::Log("SDL_GL_CONTEXT_MAJOR_VERSION: ", value);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    PJ::Log("SDL_GL_CONTEXT_MINOR_VERSION: ", value);
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value);
    PJ::Log("SDL_GL_MULTISAMPLEBUFFERS: ", value);
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &value);
    PJ::Log("SDL_GL_MULTISAMPLESAMPLES: ", value);

    world->uiEventPoller = MAKE<SDLImGuiUIEventPoller>(*window);

    auto primaryDisplayId = SDL_GetPrimaryDisplay();
    auto displayMode = SDL_GetCurrentDisplayMode(primaryDisplayId);
    if (displayMode && displayMode->refresh_rate != 0) {
        world->SetRenderRate(displayMode->refresh_rate);
    }

    SP<LoadResourcesModel> loadResourcesModel = SCAST<LoadResourcesModel>(
        MAKE<StandardLoadResourcesModel>(StandardLoadResourcesModel::LoadType::Rez)
    );
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

    auto prefab = MAKE<Prefab>([](auto& world) {
        auto result = MAKE<WorldNode>();
        QB(*result)
            .Named("Outer circle")
            .Circle(300, Themes::fruit.ThemeColor("green"))
            .And("Inner circle")
            .Circle(200, Themes::fruit.ThemeColor("blue"))
            .And("Bullseye circle")
            .Circle(100, Themes::fruit.ThemeColor("red"));

        return result;
    });

    world->prefabs["archer"] = prefab;

    auto designSystem = MAKE<DesignSystem>();
    designSystem->Add(UIElement::SliderTrack, world->FindTexture("slider-track"));
    designSystem->Add(UIElement::SliderThumb, world->FindTexture("slider-thumb"));
    designSystem->Add(UIElement::SliderVerticalTrack, world->FindTexture("slider-track-v"));
    designSystem->Add(UIElement::SliderVerticalThumb, world->FindTexture("slider-thumb-v"));
    designSystem->SetTag(
        UIElement::SliderTrack, UITag::SlicePoints,
        SlicedTextureRenderer::SlicePoints{ { 12, 12 }, { 12, 12 } }
    );
    designSystem->SetTag(UIElement::SliderTrack, UITag::EndCapSize, 20.0f);
    designSystem->Add(UIElement::ButtonSurface, world->FindTexture("example-button-normal"));
    designSystem->SetTag(
        UIElement::ButtonSurface, UITag::SlicePoints,
        SlicedTextureRenderer::SlicePoints{ { 12, 12 }, { 12, 12 } }
    );

    world->designSystem = designSystem;

    PJ::Log("Is VERBOSE flag there");

    // Register a mouse device
    Mouse::current = std::make_unique<SDLMouseDevice>();

    auto testTextureNode = MAKE<WorldNode>();
    auto testTextureScene = MAKE<TestTextureScene>();
    testTextureNode->Add(testTextureScene);
    world->Add(testTextureNode);

    auto uiSystem = MAKE<UIWorldSystem>();
    uiSystem->typeTags.insert("editor.vip");
    world->Add(uiSystem);

    auto renderSystem = MAKE<RenderWorldSystem>();
    renderSystem->typeTags.insert("editor.vip");
    world->Add(renderSystem);

    auto imGui = MAKE<ImGuiGLRenderProcessor>(renderContext->ImGuiContext());
    renderSystem->Add(imGui);

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

    auto kaijuSystem = MAKE<KaijuWorldSystem>();
    kaijuSystem->typeTags.insert("editor.vip");
    world->Add(kaijuSystem);

    auto imGuiProcessor = MAKE<KaijuImGuiRenderProcessor>(*kaijuSystem);
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
    //    overrideFeatures->map[RenderFeature::Blend] = RenderFeatureState::Disable;
    overrideFeatures->map[RenderFeature::MultiSample] = RenderFeatureState::Disable;
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
