#pragma once

#include "KaijuWorldSystem.h"
#include "SDKIncludes.h"

using namespace PJ;

void DrawSelectionWindow(WorldNode& node);

void DrawNodeInspectorWindow(KaijuWorldSystem& system);

class KaijuImGuiRenderProcessor : public RenderProcessor {
public:
    using Base = RenderProcessor;

    KaijuWorldSystem& system;

    SceneClassRegistry sceneClasses;
    bool isWindowConfigured = false;
    bool isToolActive = false;
    UP<SomeImGuiPainter> windowPainter;

    // TODO: re-evaluate all registered render phases, probably don't want to run for each camera
    KaijuImGuiRenderProcessor(KaijuWorldSystem& system) :
        Base({ .phases = { RenderPhase::PostClear } }),
        system(system) {}

    // MARK: RenderProcessor

    void Process(String phase) override;

protected:
    void BuildPainters();
};
