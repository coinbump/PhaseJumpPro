#include "App.h"
#include "QuickBuild.h"
#include "SDLPlatformClass.h"
#include "SDLWorld.h"

using namespace std;
using namespace PJ;

SP<App::World> App::world;

void NewApp() {
#ifdef DEBUG
    logLevel = LogLevel::Verbose;
#endif

    App::world = MAKE<SDLWorld>();

    SDLPlatformClass _class;
    SDLPlatformConfig& config = *_class.configs.at(ConfigId::GameEditorGL).get();

    config.windowConfig.title = "App";
    config.windowConfig.SetAllowHighDPI(true);
    config.clearColor = Color::white;
    config.resourcesPath = FilePath("resources");

    // Store SDL objects statically so they don't get destroyed
    static auto configResult = _class.Configure(config, App::world);
}

std::function<void()> App::newFunc = NewApp;

AppBuilder App::New() {
    newFunc();

    // Add a scene and standard camera
    UP<QuickBuild> qb = NEW<QuickBuild>(*App::world->Root());
    qb->And("Scene").OrthoStandard();

    auto qbPtr = qb.get();

    return AppBuilder{ .world = App::world.get(), .qb = *qbPtr, ._qb = std::move(qb) };
}

// MARK: - AppBuilder

AppBuilder::~AppBuilder() {
    GUARD(world)
    world->Go();
}
