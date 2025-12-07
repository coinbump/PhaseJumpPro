#include "App.h"
#include "QuickBuilder.h"
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
    AppBuilder ab(*App::world->Root());
    ab.And("Scene").OrthoStandard();

    return ab;
}

// MARK: - AppBuilder

void AppBuilder::OnGo() {
    Base::OnGo();

    auto world = Node().World();
    GUARD(world)
    world->Go();
}
