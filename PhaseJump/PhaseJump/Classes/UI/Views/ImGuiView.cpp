#include "ImGuiView.h"
#include "imgui.h"
#include "QuickBuilder.h"
#include "World.h"

using namespace std;
using namespace PJ;

void ImGuiView::Awake() {
    Base::Awake();

    GUARD(owner)
    auto nodeHandlerFunc = [this](auto& node) {
        auto world = node.World();
        GUARD(world)

        auto worldPos = node.transform.WorldPosition();
        auto worldSize = WorldSize();
        Vector2 topLeftWorldPos = { worldPos.x + worldSize.x * 0.5f * vecLeft,
                                    worldPos.y + worldSize.y * 0.5f * vecUp };
        Vector2 bottomRightWorldPos = { worldPos.x + worldSize.x * 0.5f * vecRight,
                                        worldPos.y + worldSize.y * 0.5f * vecDown };

        auto topLeftScreenPos = world->WorldToScreen(topLeftWorldPos);
        auto bottomRightScreenPos = world->WorldToScreen(bottomRightWorldPos);
        ImGui::SetNextWindowPos(ImVec2(topLeftScreenPos.x, topLeftScreenPos.y));
        ImGui::SetNextWindowSize(ImVec2(
            bottomRightScreenPos.x - topLeftScreenPos.x, bottomRightScreenPos.y - topLeftScreenPos.y
        ));

        bool isVisible = true;

        ImGui::Begin(
            String(title).c_str(), &isVisible,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        );

        if (paintFunc) {
            paintFunc();
        }

        ImGui::End();
    };

    AddSignalHandler({ .id = "render.immediate",
                       .func = [this, nodeHandlerFunc](auto& component, auto& signal) {
                           GUARD(component.owner)

                           nodeHandlerFunc(*component.owner);
                       } });
}
