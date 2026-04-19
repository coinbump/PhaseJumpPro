#include "EditorImGuiScenesPainter.h"
#include "EditorTypes.h"
#include "EditorWorldSystem.h"
#include "imgui.h"
#include "StringUtils.h"
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
            VectorList<char> filterBuffer;
            filterBuffer.resize(filterText.size() + 256);
            std::copy(filterText.begin(), filterText.end(), filterBuffer.begin());

            if (ImGui::InputText("Filter", filterBuffer.data(), filterBuffer.size())) {
                filterText = String(
                    filterBuffer.begin(), std::find(filterBuffer.begin(), filterBuffer.end(), 0)
                );
            }

            auto lowerFilter = ToLower(filterText);

            VectorList<SceneClass*> sceneClasses;
            for (auto& sceneClassI : system.sceneClasses.Map()) {
                auto sceneClass = sceneClassI.second.get();
                auto& name = sceneClass->_core.name;

                // ImGui requires that UI elements have names (unless you specify ##)
                GUARD_CONTINUE(name.size() > 0)

                if (!lowerFilter.empty()) {
                    GUARD_CONTINUE(ToLower(name).find(lowerFilter) != String::npos)
                }

                sceneClasses.push_back(sceneClass);
            }

            std::sort(sceneClasses.begin(), sceneClasses.end(), [](auto& lhs, auto& rhs) {
                return lhs->_core.name < rhs->_core.name;
            });

            for (auto& sceneClass : sceneClasses) {
                auto name = sceneClass->_core.name;

                if (ImGui::Button(name.c_str())) {
                    auto scene = sceneClass->Make();
                    if (scene) {
                        auto removingSystems = Filter(world.Systems(), [](auto& system) {
                            return !system->HasTypeTag(EditorTypeTag::Persist);
                        });

                        auto mappedSystems =
                            Map<SomeWorldSystem*>(removingSystems, [](auto& system) {
                                return system.get();
                            });

                        world.Remove(mappedSystems);
                        world.DestroyAllNodes();

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
