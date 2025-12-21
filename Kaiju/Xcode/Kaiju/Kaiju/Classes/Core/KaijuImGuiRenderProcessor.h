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
    UP<ImGuiPainter> windowPainter;

    KaijuImGuiRenderProcessor(KaijuWorldSystem& editorSystem) :
        Base({ .phases = { RenderPhaseId::RenderPassStartPost } }),
        editorSystem(editorSystem) {}

    // MARK: RenderProcessor

    void Process(Phase phase) override;

protected:
    void BuildPainters();
};
