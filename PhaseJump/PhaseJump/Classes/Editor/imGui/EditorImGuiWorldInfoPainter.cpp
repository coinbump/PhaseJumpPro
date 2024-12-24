#include "EditorImGuiWorldInfoPainter.h"
#include "imgui.h"
#include "World.h"

using namespace std;
using namespace PJ;

EditorImGuiWorldInfoPainter::EditorImGuiWorldInfoPainter(World& _world) :
    world(_world) {
    drawFunc = [this](auto& painter) {
        if (ImGui::CollapsingHeader("World", ImGuiTreeNodeFlags_DefaultOpen)) {
            int fps = world.renderStats.SafeValue<int>("fps");

            ImGui::Checkbox("Limit render FPS", &world.isRenderRateLimited);
            ImGui::SliderFloat("World speed", &world.timeScale, 0.0f, 5.0f);

            String fpsString = MakeString(fps);
            String fpsTitle = String("FPS: ") + fpsString;
            ImGui::TextColored(
                fps < 30 ? ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1), "%s", fpsTitle.c_str()
            );

            if (world.IsPaused()) {
                if (ImGui::Button("Play")) {
                    world.Play();
                }
            } else {
                if (ImGui::Button("Pause")) {
                    world.Pause();
                }
            }

            {
                int drawCount = world.renderStats.SafeValue<int>(RenderStatId::DrawCount);
                String countString = MakeString(drawCount);
                String countTitle = String("Draw Count: ") + countString;
                ImGui::Text("%s", countTitle.c_str());
            }

            {
                int nodeCount = world.renderStats.SafeValue<int>(RenderStatId::NodeCount);
                String countString = MakeString(nodeCount);
                String countTitle = String("Node Count: ") + countString;
                ImGui::Text("%s", countTitle.c_str());
            }

            auto& systems = world.Systems();

            ImGui::Text("Systems");

            for (size_t i = 0; i < systems.size(); i++) {
                auto& system = systems[i];
                GUARD_CONTINUE(system->Name().size() > 0)

                ImGui::Text("%s", system->Name().c_str());
            }
        }
    };
}
