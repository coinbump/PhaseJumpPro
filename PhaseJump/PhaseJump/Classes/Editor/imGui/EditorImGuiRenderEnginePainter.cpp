#include "EditorImGuiRenderEnginePainter.h"
#include "RenderProcessor.h"
#include "RenderWorldSystem.h"
#include "SomeRenderEngine.h"
#include "World.h"

using namespace std;
using namespace PJ;

EditorImGuiRenderEnginePainter::EditorImGuiRenderEnginePainter(EditorWorldSystem& _system) :
    system(_system) {
    drawFunc = [this](auto& painter) {
        auto world = *system.World();

        if (ImGui::CollapsingHeader("Render Engine", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto renderEngine = world.renderContext->renderEngine;
            if (ImGui::Checkbox(
                    "Optimize state switches", &system.renderEnginePaintModel.optimizeStateSwitches
                )) {
                renderEngine->optimizeStateSwitches =
                    system.renderEnginePaintModel.optimizeStateSwitches;
            }

            ImGui::Text("Enabled features");
            for (auto& enabledFeature : renderEngine->EnabledFeatures()) {
                ImGui::Text("%s", enabledFeature.c_str());
            }

            if (ImGui::CollapsingHeader("Render Pipeline", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto renderSystem = world.GetSystem<RenderWorldSystem>();
                GUARD(renderSystem)

                ImGui::Text("Render pipeline");

                auto const& processors = renderSystem->Processors();
                auto& switches = system.renderEnginePaintModel.renderProcessorSwitches;
                switches.resize(processors.size());
                for (size_t i = 0; i < processors.size(); i++) {
                    switches[i] = processors[i]->IsEnabled();
                }

                for (size_t i = 0; i < processors.size(); i++) {
                    auto& processor = processors[i];
                    GUARD_CONTINUE(processor->name.size() > 0)

                    if (ImGui::Checkbox(processor->name.c_str(), (bool*)&switches[i])) {
                        processor->Enable(switches[i]);
                    }
                }
            }
        }
    };
}
