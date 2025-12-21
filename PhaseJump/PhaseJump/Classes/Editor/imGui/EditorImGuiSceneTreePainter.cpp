#include "EditorImGuiSceneTreePainter.h"
#include "EditorTypes.h"
#include "EditorWorldSystem.h"
#include "imgui.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void EditorImGuiSceneTreePainter::DrawSceneTree(
    EditorWorldSystem& system, WorldNode& node, String name
) {
    if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        auto mainCamera = system.World()->MainCamera();
        bool isMainCamera = mainCamera ? node.Contains(*mainCamera) : false;
        bool isPersist = node._core.typeTags.contains(EditorTypeTag::Persist);

        ImGui::SameLine();

        if (!isMainCamera) {
            if (ImGui::SmallButton(node.IsEnabled() ? "on" : "off")) {
                node.ToggleEnable();
            }
        }

        ImGui::SameLine();

        if (!isPersist && !isMainCamera) {
            if (ImGui::SmallButton("x")) {
                system.Delete(SCAST<WorldNode>(node.shared_from_this()));
            }
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("i")) {
            system.inspectNodeModel =
                NEW<EditorWorldSystem::InspectNodeModel>(SCAST<WorldNode>(node.shared_from_this()));

            if (onInspectFunc) {
                onInspectFunc(node);
            }

            system.storage.Set<bool>("window.inspector", "isVisible", true);
        }

        Tags nameCounts;

        for (auto& childNode : node.ChildNodes()) {
            String name = childNode->Name().size() > 0 ? childNode->Name() : "???";

            auto nameCount = nameCounts.SafeValue<int>(name);
            nameCounts.Set(name, nameCount + 1);

            if (nameCount > 0) {
                std::ostringstream ss;
                ss << name << "##" << MakeString(nameCount + 1);
                name = ss.str();
            }

            DrawSceneTree(system, *childNode, name);
        }
        ImGui::TreePop();
    }
}

EditorImGuiSceneTreePainter::EditorImGuiSceneTreePainter(
    EditorWorldSystem& _system, OnInspectFunc onInspectFunc
) :
    system(_system),
    onInspectFunc(onInspectFunc) {

    drawFunc = [this](auto& painter) {
        GUARD(system.World())

        if (ImGui::CollapsingHeader("Scene Tree", ImGuiTreeNodeFlags_DefaultOpen)) {
            DrawSceneTree(system, *system.World()->Root(), "Root");
        }
    };
}
