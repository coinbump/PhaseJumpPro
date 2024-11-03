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
    config.windowConfig.title = "Kaiju";
    config.clearColor = Color(.8, .8, .8, 1.0f);
    config.resourcesPath = FilePath("resources/art");

    world->And().With<TestTextureScene>();

    // Store SDL objects statically so they don't get destroyed
    static auto configResult = _class.Configure(config, world);

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
