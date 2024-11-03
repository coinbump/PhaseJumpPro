#include "EditorImGuiPrefabsPainter.h"
#include "EditorWorldSystem.h"
#include "imgui.h"
#include "QuickBuild.h"
#include "SpriteRenderer.h"
#include "World.h"

using namespace std;
using namespace PJ;

EditorImGuiPrefabsPainter::EditorImGuiPrefabsPainter(EditorWorldSystem& _system) :
    system(_system) {

    drawFunc = [this](auto& painter) {
        auto& world = *system.World();

        if (ImGui::CollapsingHeader("Prefabs", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto& type : world.prefabs) {
                auto id = type.first;
                GUARD_CONTINUE(!IsEmpty(id))

                ImGui::Text("%s", id.c_str());

                ImGui::SameLine();

                String addButtonName("+");
                addButtonName += "##prefab-";
                addButtonName += id;

                if (ImGui::SmallButton(addButtonName.c_str())) {
                    auto prefabNode = MAKE<WorldNode>();
                    system.Add(prefabNode, world.root.get(), world.root->Children().size() - 1);
                    world.LoadPrefab(id, *prefabNode);
                    QB(*prefabNode).Drag();
                }
            }
        }
    };
}
