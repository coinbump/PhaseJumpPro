#include "RenderWorldSystem.h"
#include "RenderContextModel.h"
#include "RenderProcessingModel.h"
#include "RenderProcessor.h"
#include "SomeCamera.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

std::optional<RenderResult> RenderWorldSystem::Render(RenderContextModel& _contextModel) {
    auto context = _contextModel.renderContext;
    GUARDR(context, std::nullopt);

    context->Bind();
    context->Clear();

    int drawCount = 0;

    auto renderEngine = context->renderEngine;

    VectorList<RenderModel> models;
    models.reserve(_contextModel.nodes.size());

    {
#ifdef PROFILE
        DevProfiler devProfiler("System- Make RenderModels", [](String value) {
            std::cout << value;
        });

        cout << "Profile: Nodes count: " << contextModel.nodes.size() << std::endl;
#endif

        // FUTURE: can this be done on parallel threads?
        for (auto& node : _contextModel.nodes) {
            List<SP<SomeRenderer>> renderers;
            node->TypeComponents<SomeRenderer>(renderers);
            for (auto& renderer : renderers) {
                /// Most renderers produce render models, for a list of draw calls to be sent to the
                /// GPU by the graphics engine
                auto thisRenderModels = renderer->MakeRenderModels();
                auto thisRenderModelCount = thisRenderModels.size();

                GUARD_CONTINUE(thisRenderModelCount > 0)

                AddRange(models, thisRenderModels);
            }
        }
    }

    auto& cameras = _contextModel.cameras;

    // Execute a complete render pass for each camera
    // Example: Camera A renders moving content, camera B renders fixed UI
    // FUTURE: support limiting which nodes are rendered for each camera,
    // either by using frustrum culling or a filter
    // FUTURE: support occlusion filtering for nodes based on camera view
    for (auto& camera : cameras) {
        GUARD_CONTINUE(camera)

        RenderContextModel contextModel = _contextModel;

        // The camera can override the render context
        if (camera->renderContext) {
            contextModel.renderContext = camera->renderContext.get();
        }
        RenderSystemModel systemModel(contextModel, camera, models);

        // TODO: contextModel.renderContext->Bind();
        renderEngine->RenderStart(contextModel);
        camera->PreRender(contextModel);

#ifdef PROFILE
        DevProfiler devProfilerRenderCamera("Render- Camera", [](String value) { cout << value; });
#endif

        for (auto& processor : processingModel.Processors()) {
            GUARD_CONTINUE(processor->IsEnabled())
            processor->Process(systemModel);
        }

        RenderDrawModel drawModel;
        drawModel.models = systemModel.models;
        drawCount += (int)drawModel.models.size();
        renderEngine->RenderDraw(drawModel);
    }

    context->Present();

    RenderResult result;
    result.tags.Insert("draw.count", drawCount);
    return result;
}

void RenderWorldSystem::Add(SP<RenderProcessor> processor) {
    processingModel.Add(processor);
}

RenderWorldSystem::ProcessorList const& RenderWorldSystem::Processors() const {
    return processingModel.Processors();
}
