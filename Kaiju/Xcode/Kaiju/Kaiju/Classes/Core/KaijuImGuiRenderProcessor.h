#pragma once

#include "KaijuWorldSystem.h"
#include "SDKIncludes.h"

using namespace PJ;

void DrawSelectionWindow(WorldNode& node);

void DrawNodeInspectorWindow(KaijuWorldSystem& system);

class KaijuImGuiRenderProcessor : public RenderProcessor {
public:
    using Base = RenderProcessor;

    KaijuWorldSystem& editorSystem;

    SceneClassRegistry sceneClasses;
    bool isWindowConfigured = false;
    bool isToolActive = false;
    UP<SomeImGuiPainter> windowPainter;

    // TODO: re-evaluate all registered render phases, probably don't want to run for each camera
    KaijuImGuiRenderProcessor(KaijuWorldSystem& editorSystem) :
        Base({ .phases = { RenderPhase::PostClear } }),
        editorSystem(editorSystem) {}

    // MARK: RenderProcessor

    void Process(String phase) override;

protected:
    void BuildPainters();
};
