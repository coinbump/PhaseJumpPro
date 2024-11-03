#include "EditorImGuiInspectorWindowPainter.h"
#include "UIPlanner.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

EditorImGuiInspectorWindowPainter::EditorImGuiInspectorWindowPainter(EditorWorldSystem& _system) :
    system(_system) {
    drawFunc = [this](auto& painter) {
        auto& isInspectorWindowConfiguredStorage =
            system.imGuiStorage.ValueStorage<bool>("window.inspector", "isConfigured");
        auto& isInspectorWindowVisibleStorage =
            system.imGuiStorage.ValueStorage<bool>("window.inspector", "isVisible");

        if (!isInspectorWindowConfiguredStorage) {
            isInspectorWindowConfiguredStorage = true;

            // Show inspector for scene UI
            isInspectorWindowVisibleStorage = true;

            ImGuiIO& io = ImGui::GetIO();

            ImGui::SetNextWindowSize(ImVec2(300, io.DisplaySize.y - 40));
            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 320, 20));
        }

        if (isInspectorWindowVisibleStorage) {
            ImGui::Begin("Inspector", &isInspectorWindowVisibleStorage);

            if (system.sceneUIPlan) {
                if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGuiPlanPainter(*system.sceneUIPlan, system.imGuiStorage).Paint();
                }
            }

            if (system.inspectNodeModel && system.inspectNodeModel->node) {
                auto inspectNode = system.inspectNodeModel->node;
                auto& node = *inspectNode;

                auto nodeName = IsEmpty(node.name) ? "Node" : node.name.c_str();
                ImGui::Text("%s", nodeName);

                UIPlan uiPlan;
                UIPlanner planner(uiPlan);

                planner.InputFloat(
                    "X", Binding<float>(
                             [&]() { return node.transform.LocalPosition().x; },
                             [&](auto& value) { node.transform.SetAxisPosition(Axis::X, value); }
                         )
                );
                planner.InputFloat(
                    "Y", Binding<float>(
                             [&]() { return node.transform.LocalPosition().y; },
                             [&](auto& value) { node.transform.SetAxisPosition(Axis::Y, value); }
                         )
                );
                planner.InputFloat(
                    "Z", Binding<float>(
                             [&]() { return node.transform.LocalPosition().z; },
                             [&](auto& value) { node.transform.SetAxisPosition(Axis::Z, value); }
                         )
                );

                ImGuiPlanPainter(uiPlan, system.inspectNodeModel->imGuiStorage).Paint();

                // FUTURE: add 3D support if needed
                auto& rotationStorage =
                    system.imGuiStorage.ValueStorage<float>("inspect", "rotation2D");
                rotationStorage = -node.transform.Rotation().z;
                if (ImGui::SliderFloat("Rotation", &rotationStorage, 0, 360.0f)) {
                    node.transform.SetRotation({ 0, 0, -rotationStorage });
                }

                // FUTURE: add 3D support if needed
                auto& scaleUniformStorage =
                    system.imGuiStorage.ValueStorage<float>("inspect", "scale2D.uniform");
                scaleUniformStorage = node.transform.Scale().x;
                if (ImGui::SliderFloat("Scale", &scaleUniformStorage, 0.5f, 5.0f)) {
                    node.transform.SetScale({ scaleUniformStorage, scaleUniformStorage, 1 });
                }

                ImGui::Text("Components");
                for (auto& component : inspectNode->Components()) {
                    SomeWorldComponent* id = component.get();
                    auto name = component->Name();
                    GUARD_CONTINUE(!IsEmpty(name))

                    try {
                        auto& uiPlan = system.inspectNodeModel->componentUIPlans.at(id);
                        if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGuiPlanPainter(*uiPlan.get(), system.inspectNodeModel->imGuiStorage)
                                .Paint();
                        }
                    } catch (...) {
                        ImGui::Text("%s", component->Name().c_str());
                    }
                }
            }

            ImGui::End();
        }
    };
}
