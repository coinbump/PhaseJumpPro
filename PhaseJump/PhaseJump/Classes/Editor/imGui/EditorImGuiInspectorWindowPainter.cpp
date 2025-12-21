#include "EditorImGuiInspectorWindowPainter.h"
#include "UIPlanner.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

EditorImGuiInspectorWindowPainter::EditorImGuiInspectorWindowPainter(EditorWorldSystem& _system) :
    system(_system) {
    drawFunc = [this](auto& painter) {
        bool isInspectorWindowConfigured =
            system.storage.SafeValue<bool>("window.inspector", "isConfigured");
        bool isInspectorWindowVisible =
            system.storage.SafeValue<bool>("window.inspector", "isVisible");

        if (!isInspectorWindowConfigured) {
            system.storage.Set("window.inspector", "isConfigured", true);

            // Show inspector for scene UI
            isInspectorWindowVisible = true;

            ImGuiIO& io = ImGui::GetIO();

            ImGui::SetNextWindowSize(ImVec2(300, io.DisplaySize.y - 40));
            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 320, 20));
        }

        if (isInspectorWindowVisible) {
            ImGui::Begin("Inspector", &isInspectorWindowVisible);
            system.storage.Set("window.inspector", "isVisible", isInspectorWindowVisible);

            if (system.sceneUIPlan) {
                ImGuiPushId push("scene");

                if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGuiUIPlanPainter(*system.sceneUIPlan).Paint();
                }
            }

            if (system.inspectNodeModel && !system.inspectNodeModel->node.expired()) {
                auto inspectNode = system.inspectNodeModel->node.lock();
                auto& node = *inspectNode;

                {
                    ImGuiPushId push("node");

                    String nodeName = IsEmpty(node.Name()) ? "Node" : node.Name();
                    ImGui::Text("%s", nodeName.c_str());

                    UIPlan uiPlan;
                    UIPlanner planner(uiPlan);

                    planner.InputFloat({ .label = "X",
                                         .binding = Binding<float>(
                                             [&]() { return node.transform.LocalPosition().x; },
                                             [&](auto& value) {
                                                 node.transform.SetAxisPosition(Axis::X, value);
                                             }
                                         ) });
                    planner.InputFloat({ .label = "Y",
                                         .binding = Binding<float>(
                                             [&]() { return node.transform.LocalPosition().y; },
                                             [&](auto& value) {
                                                 node.transform.SetAxisPosition(Axis::Y, value);
                                             }
                                         ) });
                    planner.InputFloat({ .label = "Z",
                                         .binding = Binding<float>(
                                             [&]() { return node.transform.LocalPosition().z; },
                                             [&](auto& value) {
                                                 node.transform.SetAxisPosition(Axis::Z, value);
                                             }
                                         ) });

                    ImGuiUIPlanPainter(uiPlan).Paint();

                    // FUTURE: add 3D support if needed
                    float rotation = -node.transform.Rotation().z;
                    if (ImGui::SliderFloat("Rotation", &rotation, 0, 360.0f)) {
                        node.transform.SetRotation({ 0, 0, -rotation });
                    }

                    // FUTURE: add 3D support if needed
                    float scaleUniform = node.transform.Scale().x;
                    if (ImGui::SliderFloat("Scale", &scaleUniform, 0.5f, 5.0f)) {
                        node.transform.SetScale({ scaleUniform, scaleUniform, 1 });
                    }

                    auto scaleString = MakeString(
                        "X: ", node.transform.Scale().x, " Y: ", node.transform.Scale().y,
                        " Z: ", node.transform.Scale().z
                    );
                    ImGui::Text("%s", scaleString.c_str());

                    if (node.IsListener()) {
                        ImGui::Text("Is Listener");
                    }
                }

                {
                    ImGuiPushId push("components");

                    ImGui::Text("Components");

                    for (auto& component : inspectNode->Components()) {
                        auto name = component->Name();
                        GUARD_CONTINUE(!IsEmpty(name))

                        ImGuiPushId push(component.get());

                        auto uiPlan = component->MakeUIPlan(UIContextId::Inspector);
                        if (uiPlan) {
                            if (ImGui::CollapsingHeader(
                                    name.c_str(), ImGuiTreeNodeFlags_DefaultOpen
                                )) {
                                ImGuiUIPlanPainter(*uiPlan.get()).Paint();
                            }
                        } else {
                            ImGui::Text("%s", component->Name().c_str());
                        }
                    }
                }
            }

            ImGui::End();
        }
    };
}
