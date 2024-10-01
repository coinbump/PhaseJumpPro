#include "KaijuImGuiRenderProcessor.h"

using namespace std;
using namespace PJ;

// auto positionXItem = ImGuiItems::MakeInputFloatItem("X", {[](})

void DrawSelectionWindow(WorldNode& node) {
    // TODO: which is selected?
    auto& components = node.Components();

    static char text[1000];

    std::ostringstream ss;
    ss << node.transform.LocalPosition();

    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Position: %s", ss.str().c_str());

    size_t i = 0;
    std::for_each(components.begin(), components.end(), [&](SP<SomeWorldComponent> component) {
        auto typeName = typeid(component).name();

        auto textRenderer = As<TextRenderer>(component.get());
        if (textRenderer) {
            std::strcpy(text, textRenderer->Text().c_str());

            ImGui::InputText("Text", text, 1000);

            textRenderer->SetText(text);
        }

        //        GUARD(typeName.size() > 0)

        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", typeName);

        i++;
    });
}

void DrawInspectedNodeWindow(KaijuWorldSystem& system) {
    auto inspectNode = system.inspectedNode;
    GUARD(inspectNode)

    auto& node = *inspectNode;

    if (!system.isInspectWindowConfigured) {
        system.isInspectWindowConfigured = true;

        ImGuiIO& io = ImGui::GetIO();

        ImGui::SetNextWindowSize(ImVec2(300, io.DisplaySize.y - 40));
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 320, 20));
    }

    ImGui::Begin("Inspect", nullptr, 0);

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Node");

    // FUTURE: ImGuiBindings::InputFloat(system.inspectPosX, ImGUiBindings::MakePositionXBinding())

    // TODO: not great to have the plan leave memory??
    //    UIPlan uiPlan;
    //    UIPlanner planner(uiPlan);
    //
    //    planner.InputFloat(
    //        "X", Binding<float>(
    //                 [&]() { return node.transform.LocalPosition().x; },
    //                 [&](auto& value) {
    //                     auto localPos = node.transform.LocalPosition();
    //                     localPos.AxisValue(Axis::X) = value;
    //                     node.transform.SetLocalPosition(localPos);
    //                 }
    //             )
    //    );
    //    planner.InputFloat(
    //        "Y", Binding<float>(
    //                 [&]() { return node.transform.LocalPosition().y; },
    //                 [&](auto& value) {
    //                     auto localPos = node.transform.LocalPosition();
    //                     localPos.AxisValue(Axis::Y) = value;
    //                     node.transform.SetLocalPosition(localPos);
    //                 }
    //             )
    //    );
    //    planner.InputFloat(
    //        "Z", Binding<float>(
    //                 [&]() { return node.transform.LocalPosition().z; },
    //                 [&](auto& value) {
    //                     auto localPos = node.transform.LocalPosition();
    //                     localPos.AxisValue(Axis::Z) = value;
    //                     node.transform.SetLocalPosition(localPos);
    //                 }
    //             )
    //    );
    //
    //    ImGuiPlanPainter().Draw(uiPlan);

    system.inspectRotation = -node.transform.Rotation().z;
    if (ImGui::SliderFloat("Rotation", &system.inspectRotation, 0, 360.0f)) {
        node.transform.SetRotation({ 0, 0, -system.inspectRotation });
    }

    system.inspectScale = node.transform.Scale().x;
    if (ImGui::SliderFloat("Scale", &system.inspectScale, 0.5f, 5.0f)) {
        node.transform.SetScale({ system.inspectScale, system.inspectScale, 1 });
    }

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Components");
    for (auto& component : system.inspectedNode->Components()) {

        SomeWorldComponent* id = component.get();

        //        GUARD(typeName.size() > 0)
        try {
            auto& uiPlan = system.inspectUIPlans.at(id);
            if (ImGui::CollapsingHeader(
                    component->Name().c_str(), ImGuiTreeNodeFlags_DefaultOpen
                )) {
                ImGuiPlanPainter(system.inspectStorage).Draw(*uiPlan.get());
            }
        } catch (...) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", component->Name().c_str());
        }
    }

    //    static float colors[4] = { 1, 1, 1, 1 };
    //    ImGui::ColorEdit4("Color", &colors[0], ImGuiColorEditFlags_DisplayRGB);

    ImGui::End();
}

void DrawTree(KaijuWorldSystem& system, SP<WorldNode> node, String name) {
    GUARD(node);

    // TODO: move this to a system?
    if (ImGui::TreeNodeEx(name.c_str())) {
        //        bool isScene = !IsEmpty(node->GetComponents<Scene>());
        bool isCamera = !IsEmpty(node->GetComponents<SomeCamera>());
        bool isVIP = node->core.typeTags.contains("editor.vip");

        ImGui::SameLine();
        //        if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
        //            pressed_count += 1;

        // FUTURE: use imageButton
        if (!isCamera) {
            if (ImGui::SmallButton(node->IsEnabled() ? "on" : "off")) {
                node->ToggleEnable();
            }
        }

        ImGui::SameLine();

        if (!isVIP) {
            if (ImGui::SmallButton("x")) {
                DeleteNodeCommandCore commandCore{ node, node->Parent() };
                auto command = MAKE<Command<DeleteNodeCommandCore>>(
                    String("Delete ") + node->name, commandCore,
                    [](auto& command) { command.core.node->Destroy(); },
                    [](auto& command) {
                        command.core.node->Restore();
                        command.core.parent->Add(command.core.node);
                    }
                );
                system.commands.Run(command);
            }
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("i")) {
            system.inspectedNode = node;
            system.inspectUIPlans.clear();
            system.inspectStorage = {};

            for (auto& component : node->Components()) {
                auto plan = component->MakeUIPlan("inspect");
                if (plan) {
                    SomeWorldComponent* id = component.get();
                    system.inspectUIPlans[id] = std::move(plan);
                }
            }
        }

        Tags nameCounts;

        for (auto& childNode : node->ChildNodes()) {
            String name = childNode->name.size() > 0 ? childNode->name : "???";

            auto nameCount = nameCounts.SafeValue<int>(name);
            nameCounts.Insert(name, nameCount + 1);

            if (nameCount > 0) {
                std::ostringstream ss;
                ss << name << " (" << MakeString(nameCount + 1) << ")";
                name = ss.str();
            }

            DrawTree(system, childNode, name);
        }
        ImGui::TreePop();
    }
}
