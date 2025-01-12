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
        auto& world = *system.World();

        if (ImGui::CollapsingHeader("Render Engine", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto& renderEngine = world.renderContext->renderEngine;
            bool optimizeStateSwitches = renderEngine.optimizeStateSwitches;
            if (ImGui::Checkbox("Optimize state switches", &optimizeStateSwitches)) {
                renderEngine.optimizeStateSwitches = optimizeStateSwitches;
            }

            ImGui::Text("Enabled features");
            for (auto& enabledFeature : renderEngine.EnabledFeatures()) {
                ImGui::Text("%s", enabledFeature.c_str());
            }

            auto drawRenderProcessors = [&](String label,
                                            VectorList<SP<RenderProcessor>> const& processors) {
                if (!IsEmpty(label)) {
                    ImGui::Text("%s", label.c_str());
                }

                for (auto& processor : processors) {
                    GUARD_CONTINUE(processor->name.size() > 0)

                    bool value = processor->IsEnabled();

                    if (ImGui::Checkbox(processor->name.c_str(), &value)) {
                        processor->Enable(value);
                    }
                }
            };

            if (ImGui::CollapsingHeader("Render Pipeline", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto renderSystem = world.GetSystem<RenderWorldSystem>();
                GUARD(renderSystem)

                drawRenderProcessors("Render Processors", renderSystem->Processors());
            }

            // FUTURE: show render options for other cameras
            auto mainCamera = world.MainCamera();
            if (mainCamera) {
                drawRenderProcessors("Main Camera", mainCamera->processingModel.Processors());
            }
        }
    };
}

// MARK: - EditorImGuiRenderPipelinePainter

EditorImGuiRenderPipelinePainter::EditorImGuiRenderPipelinePainter(
    World& _world, RenderProcessingModel::ProcessorList& processors
) :
    world(_world),
    processors(processors) {
    drawFunc = [this](auto& painter) {
        auto drawRenderProcessors = [&](String label,
                                        VectorList<SP<RenderProcessor>> const& processors) {
            if (!IsEmpty(label)) {
                ImGui::Text("%s", label.c_str());
            }

            for (auto& processor : processors) {
                GUARD_CONTINUE(processor->name.size() > 0)

                bool value = processor->IsEnabled();

                if (ImGui::Checkbox(processor->name.c_str(), &value)) {
                    processor->Enable(value);
                }
            }
        };

        if (ImGui::CollapsingHeader("Render Pipeline", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto renderSystem = world.GetSystem<RenderWorldSystem>();
            GUARD(renderSystem)

            drawRenderProcessors("Render Processors", renderSystem->Processors());
        }
    };
}
