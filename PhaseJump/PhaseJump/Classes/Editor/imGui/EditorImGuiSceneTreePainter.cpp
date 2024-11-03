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
        bool isPersist = node.core.typeTags.contains(EditorTypeTag::Persist);

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
            system.inspectNodeModel = NEW<EditorWorldSystem::InspectNodeModel>(&node);

            for (auto& component : node.Components()) {
                auto plan = component->MakeUIPlan(UIContextId::Inspector);
                if (plan) {
                    SomeWorldComponent* id = component.get();
                    system.inspectNodeModel->componentUIPlans.insert_or_assign(id, std::move(plan));
                }
            }

            if (onInspectFunc) {
                onInspectFunc(node);
            }

            auto& isInspectorWindowVisibleStorage =
                system.imGuiStorage.ValueStorage<bool>("window.inspector", "isVisible");
            isInspectorWindowVisibleStorage = true;
        }

        Tags nameCounts;

        for (auto& childNode : node.ChildNodes()) {
            String name = childNode->name.size() > 0 ? childNode->name : "???";

            auto nameCount = nameCounts.SafeValue<int>(name);
            nameCounts.Insert(name, nameCount + 1);

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
            DrawSceneTree(system, *system.World()->root, "Root");
        }
    };
}
