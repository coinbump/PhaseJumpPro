#include "EditorImGuiScenesPainter.h"
#include "EditorTypes.h"
#include "EditorWorldSystem.h"
#include "imgui.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

EditorImGuiScenesPainter::EditorImGuiScenesPainter(
    EditorWorldSystem& _system, OnSceneSwitchFunc _onSceneSwitchFunc
) :
    system(_system),
    onSceneSwitchFunc(_onSceneSwitchFunc) {

    drawFunc = [this](auto& painter) {
        auto& world = *system.World();

        if (ImGui::CollapsingHeader("Scenes", ImGuiTreeNodeFlags_DefaultOpen)) {
            VectorList<SceneClass*> sceneClasses;
            for (auto& sceneClassI : system.sceneClasses.map) {
                sceneClasses.push_back(sceneClassI.second.get());
            }

            std::sort(sceneClasses.begin(), sceneClasses.end(), [](auto& lhs, auto& rhs) {
                return lhs->core.name < rhs->core.name;
            });

            for (auto& sceneClass : sceneClasses) {
                auto name = sceneClass->core.name;

                // ImGui requires that UI elements have names (unless you specify ##)
                GUARD_CONTINUE(name.size() > 0)

                if (ImGui::Button(name.c_str())) {
                    auto scene = sceneClass->Make();
                    if (scene) {
                        auto removingSystems =
                            Filter(world.Systems(), [](SP<SomeWorldSystem> system) {
                                return !system->typeTags.contains(EditorTypeTag::Persist);
                            });

                        world.Remove(removingSystems);
                        world.RemoveAllNodes();

                        auto sceneNode = MAKE<WorldNode>();
                        sceneNode->Add(scene);
                        world.Add(sceneNode);

                        system.commands.Clear();
                        system.inspectNodeModel = {};
                        system.sceneUIPlan = scene->MakeUIPlan(UIContextId::Editor);

                        if (onSceneSwitchFunc) {
                            onSceneSwitchFunc(*scene);
                        }
                    }
                }
            }
        }
    };
}
