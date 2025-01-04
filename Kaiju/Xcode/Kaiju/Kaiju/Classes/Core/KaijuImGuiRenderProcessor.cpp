#include "KaijuImGuiRenderProcessor.h"
#include <algorithm>

using namespace std;
using namespace PJ;

// auto positionXItem = ImGuiItems::MakeInputFloatItem("X", {[](})

void KaijuImGuiRenderProcessor::Process(Phase phase) {
    GUARD(editorSystem.isUIVisible)
    GUARD_LOG(nullptr == phase.cameraModel, "ERROR. Don't process imGui for each camera")

    BuildPainters();

    EditorImGuiInspectorWindowPainter(editorSystem).Paint();

    bool isToolsWindowConfigured =
        editorSystem.storage.SafeValue<bool>("window.tools", "isConfigured");
    if (!isToolsWindowConfigured) {
        editorSystem.storage.Set("window.tools", "isConfigured", true);

        ImGuiIO& io = ImGui::GetIO();

        ImGui::SetNextWindowSize(ImVec2(300, io.DisplaySize.y - 40));
        ImGui::SetNextWindowPos(ImVec2(20, 20));
    }

    if (windowPainter) {
        windowPainter->Paint();
    }
}

void KaijuImGuiRenderProcessor::BuildPainters() {
    GUARD(nullptr == windowPainter)

    auto world = editorSystem.World();
    GUARD(world)

    windowPainter = NEW<ImGuiWindowPainter>("Kaiju", &isToolActive);
    UP<SomeImGuiPainter> worldInfoPainter = NEW<EditorImGuiWorldInfoPainter>(*world);
    UP<SomeImGuiPainter> menuBarPainter = NEW<ImGuiMainMenuBarPainter>(editorSystem.menus);

    UP<SomeImGuiPainter> scrollingPainter = NEW<ImGuiChildPainter>("Scrolling");

    UP<SomeImGuiPainter> scenesPainter =
        NEW<EditorImGuiScenesPainter>(editorSystem, EditorImGuiScenesPainter::OnSceneSwitchFunc());

    UP<SomeImGuiPainter> prefabsPainter = NEW<EditorImGuiPrefabsPainter>(editorSystem);
    UP<SomeImGuiPainter> resourcesPainter = NEW<EditorImGuiResourcesPainter>(editorSystem);
    UP<SomeImGuiPainter> sceneTreePainter = NEW<EditorImGuiSceneTreePainter>(
        editorSystem, EditorImGuiSceneTreePainter::OnInspectFunc()
    );
    UP<SomeImGuiPainter> renderEnginePainter = NEW<EditorImGuiRenderEnginePainter>(editorSystem);
    UP<SomeImGuiPainter> commandHistoryPainter =
        NEW<EditorImGuiCommandHistoryPainter>(editorSystem.commands);

    scrollingPainter->tree.Add(scenesPainter);
    scrollingPainter->tree.Add(prefabsPainter);
    scrollingPainter->tree.Add(resourcesPainter);
    scrollingPainter->tree.Add(sceneTreePainter);
    scrollingPainter->tree.Add(renderEnginePainter);
    scrollingPainter->tree.Add(commandHistoryPainter);

    windowPainter->tree.Add(menuBarPainter);
    windowPainter->tree.Add(worldInfoPainter);
    windowPainter->tree.Add(scrollingPainter);
}
