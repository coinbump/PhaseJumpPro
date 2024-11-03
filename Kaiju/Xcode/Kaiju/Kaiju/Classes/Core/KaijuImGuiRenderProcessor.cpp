#include "KaijuImGuiRenderProcessor.h"
#include <algorithm>

using namespace std;
using namespace PJ;

// auto positionXItem = ImGuiItems::MakeInputFloatItem("X", {[](})

void KaijuImGuiRenderProcessor::Process(String phase) {
    BuildPainters();

    EditorImGuiInspectorWindowPainter(system).Paint();

    auto& isToolsWindowConfiguredStorage =
        system.imGuiStorage.ValueStorage<bool>("window.tools", "isConfigured");
    if (!isToolsWindowConfiguredStorage) {
        isToolsWindowConfiguredStorage = true;

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

    auto world = system.World();
    GUARD(world)

    windowPainter = NEW<ImGuiWindowPainter>("Kaiju", &isToolActive);
    UP<SomeImGuiPainter> worldInfoPainter = NEW<EditorImGuiWorldInfoPainter>(*world);
    UP<SomeImGuiPainter> menuBarPainter = NEW<ImGuiMainMenuBarPainter>(system.menus);

    UP<SomeImGuiPainter> scrollingPainter = NEW<ImGuiChildPainter>("Scrolling");

    UP<SomeImGuiPainter> scenesPainter =
        NEW<EditorImGuiScenesPainter>(system, EditorImGuiScenesPainter::OnSceneSwitchFunc());

    UP<SomeImGuiPainter> prefabsPainter = NEW<EditorImGuiPrefabsPainter>(system);
    UP<SomeImGuiPainter> resourcesPainter = NEW<EditorImGuiResourcesPainter>(system);
    UP<SomeImGuiPainter> sceneTreePainter =
        NEW<EditorImGuiSceneTreePainter>(system, EditorImGuiSceneTreePainter::OnInspectFunc());
    UP<SomeImGuiPainter> renderEnginePainter = NEW<EditorImGuiRenderEnginePainter>(system);
    UP<SomeImGuiPainter> commandHistoryPainter =
        NEW<EditorImGuiCommandHistoryPainter>(system.commands);

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
