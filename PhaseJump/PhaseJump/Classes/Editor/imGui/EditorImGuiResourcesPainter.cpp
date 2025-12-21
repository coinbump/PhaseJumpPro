#include "EditorImGuiResourcesPainter.h"
#include "EditorWorldSystem.h"
#include "imgui.h"
#include "QuickBuilder.h"
#include "SpriteRenderer.h"
#include "World.h"

using namespace std;
using namespace PJ;

EditorImGuiResourcesPainter::EditorImGuiResourcesPainter(EditorWorldSystem& _system) :
    system(_system) {

    drawFunc = [this](auto& painter) {
        auto& world = *system.World();

        if (ImGui::CollapsingHeader("Resources", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto& type : world.resources.Map()) {
                auto name = type.first.c_str();
                GUARD_CONTINUE(name)

                if (ImGui::CollapsingHeader(name)) {
                    for (auto& res : type.second) {
                        auto resourceId = res.second.info.id;
                        ImGui::Text(
                            "%s, Used: %ld", resourceId.c_str(), res.second.resource.use_count()
                        );

                        // Allow user to manually add textures into scene
                        if (String(name) == String(ResourceType::Texture)) {
                            ImGui::SameLine();

                            String addButtonName("+");
                            addButtonName += "##texture-";
                            addButtonName += resourceId;

                            if (ImGui::SmallButton(addButtonName.c_str())) {
                                QB(*world.Root())
                                    .And()
                                    .With<SpriteRenderer>(SCAST<Texture>(res.second.resource))
                                    .Drag();
                            }
                        }
                    }
                }
            }
        }
    };
}
