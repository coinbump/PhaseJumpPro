#include "SDLPlatformClass.h"
#include "BatchByMaterialRenderProcessor.h"
#include "EditorTypes.h"
#include "GLRenderEngine.h"
#include "ImGuiGLRenderProcessor.h"
#include "LoadResourcesModel.h"
#include "RenderWorldSystem.h"
#include "ResourceRepository.h"
#include "ResourceScanner.h"
#include "SDLFileManager.h"
#include "SDLPlatformConfig.h"
#include "SDLPlatformWindow.h"
#include "SDLPlatformWindowConfig.h"
#include "SDLUIEventPoller.h"
#include "SDLWorld.h"
#include "ShowBoundsRenderProcessor.h"
#include "ShowCollidersRenderProcessor.h"
#include "ShowMeshRenderProcessor.h"
#include "StandardLoadResourcesModel.h"
#include "UIWorldSystem.h"

using namespace std;
using namespace PJ;

SDLPlatformClass::SDLPlatformClass() {
    {
        auto _config = NEW<SDLPlatformConfig>();
        auto& config = *_config.get();
        config.initFlags = SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC;

        auto windowConfig = SDLPlatformWindowConfig::openGL;
        windowConfig.SetIsResizable(true);
        windowConfig.SetIsFullscreenDesktop(true);
        config.windowConfig = windowConfig;

        configs[ConfigId::GameEditorGL] = std::move(_config);
    }

    {
        auto _config = NEW<SDLPlatformConfig>();
        auto& config = *_config.get();
        config.initFlags = SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC;

        auto windowConfig = SDLPlatformWindowConfig::openGL;
        windowConfig.SetIsResizable(false);
        windowConfig.SetIsFullscreen(true);
        config.windowConfig = windowConfig;

        configs[ConfigId::GameReleaseGL] = std::move(_config);
    }

    // FUTURE: add more configs as needed
}

SDLConfigPlatformResult SDLPlatformClass::Configure(SDLPlatformConfig& config, SP<SDLWorld> world) {
    GUARDR_LOG(world, { {} }, "ERROR. Can't create SDL window without world")

    SDL_Init(config.initFlags);

    if (IsLogVerbose()) {
        // Enable standard application logging
        SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    }

    auto window = MAKE<SDLPlatformWindow>(world);
    window->Configure(config.windowConfig);

    int pixelW{}, pixelH{};
    SDL_GetWindowSizeInPixels(window->SDLWindow(), &pixelW, &pixelH);

    PJ::Log("SDL: Window display scale: ", SDL_GetWindowDisplayScale(window->SDLWindow()));
    // FUTURE: PJ::Log("SDL: Display content scale: ", SDL_GetDisplayContentScale(display)));
    PJ::Log("SDL: Window pixel density: ", SDL_GetWindowPixelDensity(window->SDLWindow()));
    PJ::Log("SDL: Window size in pixels: ", pixelW, ", ", pixelH);
    PJ::Log("SDL: Is Joystick Enabled?: ", SDL_JoystickEventsEnabled());
    PJ::Log("SDL: Is Gamepad Enabled?: ", SDL_GamepadEventsEnabled());

    // FUTURE: support non-OpenGL render SDKs if needed
    SP<GLRenderEngine> glRenderEngine = MAKE<GLRenderEngine>();

    auto renderContext = MAKE<SDLGLRenderContext>(glRenderEngine);
    renderContext->clearColor = config.clearColor;
    renderContext->Configure(window->SDLWindow());

    // Create context before render engine can start
    glRenderEngine->Go();

    // Connect the world
    world->Configure(window->SDLWindow(), renderContext);

    // Poll for UI events
    world->uiEventPoller = MAKE<SDLUIEventPoller>(window->SDLWindow());

    // Log SDL render values
    int value{};
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
    PJ::Log("SDL: SDL_GL_DOUBLEBUFFER: ", value);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
    PJ::Log("SDL: SDL_GL_DEPTH_SIZE: ", value);
    SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &value);
    PJ::Log("SDL: SDL_GL_STENCIL_SIZE: ", value);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    PJ::Log("SDL: SDL_GL_CONTEXT_MAJOR_VERSION: ", value);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    PJ::Log("SDL: SDL_GL_CONTEXT_MINOR_VERSION: ", value);
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value);
    PJ::Log("SDL: SDL_GL_MULTISAMPLEBUFFERS: ", value);
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &value);
    PJ::Log("SDL: SDL_GL_MULTISAMPLESAMPLES: ", value);

    // Set the world's render rate to match the monitor's refresh rate
    if (config.renderRate) {
        world->SetRenderRate(*config.renderRate);
    } else {
        // FUTURE: add support for moving between monitors with different refresh rates
        auto primaryDisplayId = SDL_GetPrimaryDisplay();
        auto displayMode = SDL_GetCurrentDisplayMode(primaryDisplayId);
        if (displayMode && displayMode->refresh_rate != 0) {
            world->SetRenderRate(displayMode->refresh_rate);
        }
    }

    // Load initial resources
    // Careful: this is only meant for quick prototyping. It will block the main thread
    if (config.resourcesPath.string().size() > 0) {
        SP<LoadResourcesModel> loadResourcesModel =
            MAKE<StandardLoadResourcesModel>(StandardLoadResourcesModel::LoadType::Rez);

        SP<FileManager> fm = MAKE<SDLFileManager>();
        // TODO: scanner doesn't need SP
        ResourceScanner resourceScanner(loadResourcesModel, fm);
        ResourceRepository resourceRepository(loadResourcesModel, world->loadedResources, fm);

        FilePath path = SDL_GetBasePath();
        path /= config.resourcesPath;

        auto loadResourcesPlan = resourceScanner.ScanAt(path, FileSearchType::Recursive);
        auto allInfos = loadResourcesPlan.AllInfos();
        for (auto& info : allInfos) {
            resourceRepository.Load(info);
        }
    }

    // Register a mouse device
    Mouse::current = NEW<SDLMouseDevice>();

    auto uiSystem = MAKE<UIWorldSystem>();
    uiSystem->typeTags.insert(EditorTypeTag::Persist);
    world->Add(uiSystem);

    auto renderSystem = MAKE<RenderWorldSystem>();
    renderSystem->typeTags.insert(EditorTypeTag::Persist);
    world->Add(renderSystem);

    // FUTURE: config should specify which render processes to use
    auto imGuiRenderProcessor = MAKE<ImGuiGLRenderProcessor>(renderContext->ImGuiContext());
    renderSystem->Add(imGuiRenderProcessor);

    auto showMeshRenderProcessor = MAKE<ShowMeshRenderProcessor>();
    showMeshRenderProcessor->Enable(false);
    renderSystem->Add(showMeshRenderProcessor);

    auto showCollidersRenderProcessor = MAKE<ShowCollidersRenderProcessor>();
    showCollidersRenderProcessor->Enable(false);
    renderSystem->Add(showCollidersRenderProcessor);

    auto showBoundsRenderProcessor = MAKE<ShowBoundsRenderProcessor>();
    showBoundsRenderProcessor->Enable(false);
    renderSystem->Add(showBoundsRenderProcessor);

    // Important: always batch process, or renders will be slow
    auto batchRenderProcessor = MAKE<BatchByMaterialRenderProcessor>();
    renderSystem->Add(batchRenderProcessor);

    return { window };
}
