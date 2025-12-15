#ifndef _TESTS_

#include "SDLKaiju.h"
#include "KaijuImGuiRenderProcessor.h"
#include "SDKIncludes.h"
#include "TestTextureScene.h"
#include <iostream>

using namespace PJ;
using namespace std;

class KaijuWorld : public SDLWorld {
public:
    using Base = SDLWorld;
    using This = KaijuWorld;

protected:
};

void KaijuGo() {
#ifdef DEBUG
    logLevel = LogLevel::Verbose;
#endif

    auto world = MAKE<KaijuWorld>();

    auto prefab = MAKE<Prefab>([](auto& node) {
        QB(node)
            .Named("Outer circle")
            .Circle(300, Themes::fruit.ThemeColor("green"))
            .And("Inner circle")
            .Circle(200, Themes::fruit.ThemeColor("blue"))
            .And("Bullseye circle")
            .Circle(100, Themes::fruit.ThemeColor("red"));
    });

    world->prefabs["archer"] = prefab;

    SDLPlatformClass _class;
    auto& config = *_class.configs.at(ConfigId::GameEditorGL).get();

#ifdef DEBUG
    config.windowConfig.title = "Kaiju-Dev";
#else
    config.windowConfig.title = "Kaiju";
#endif

    config.windowConfig.SetAllowHighDPI(true);
    config.clearColor = Color(.8, .8, .8, 1.0f);
    config.resourcesPath = FilePath("resources");

    world->And().With<TestTextureScene>();

    // Store SDL objects statically so they don't get destroyed
    static auto configResult = _class.Configure(config, world);

    auto _designSystem = NEW<DuckDesignSystem>();
    auto designSystem = _designSystem.get();
    designSystem->AddElementTexture(
        UIElementId::SliderTrack, world->resources.FindTexture("slider-track")
    );
    designSystem->AddElementTexture(
        UIElementId::SliderThumb, world->resources.FindTexture("slider-thumb")
    );
    designSystem->AddElementTexture(
        UIElementId::SliderVerticalTrack, world->resources.FindTexture("slider-track-v")
    );
    designSystem->AddElementTexture(
        UIElementId::SliderVerticalThumb, world->resources.FindTexture("slider-thumb-v")
    );
    designSystem->theme->SetElementTag(
        UIElementId::SliderTrack, UITag::Slice9Model,
        Slice9TextureRenderer::SliceModel{ .topLeftInsets = { 10, 10 },
                                           .bottomRightInsets = { 10, 10 } }
    );
    designSystem->theme->SetElementTag(UIElementId::SliderTrack, UITag::EndCapSize, 20.0f);
    world->designSystem = std::move(_designSystem);

    auto kaijuSystem = MAKE<KaijuWorldSystem>();
    world->Add(kaijuSystem);

    auto renderSystem = world->TypeSystem<RenderWorldSystem>();

    auto kaijuImGuiRenderProcessor = MAKE<KaijuImGuiRenderProcessor>(*kaijuSystem);
    renderSystem->Add(kaijuImGuiRenderProcessor);

    auto overrideFeaturesRenderProcessor = MAKE<OverrideFeaturesRenderProcessor>();
    overrideFeaturesRenderProcessor->map[RenderFeature::MultiSample] = RenderFeatureState::Disable;
    overrideFeaturesRenderProcessor->Enable(false);
    renderSystem->Add(overrideFeaturesRenderProcessor);

    //    int count = 0;
    //    SDL_PenID* result = SDL_GetPens(&count);

    world->Go();
}

#endif
