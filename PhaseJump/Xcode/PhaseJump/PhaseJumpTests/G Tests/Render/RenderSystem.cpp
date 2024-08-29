#include "RenderSystem.h"
#include "RenderContextModel.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "SomeCamera.h"
#include "RenderProcessor.h"
#include "SomeRenderer.h"

using namespace std;
using namespace PJ;

std::optional<RenderResult> RenderSystem::Render(RenderContextModel& contextModel) {
    auto context = contextModel.renderContext;
    GUARDR(context, std::nullopt);

    RenderSystemModel systemModel(contextModel);

    context->Bind();
    context->Clear();

    int drawCount = 0;

    auto renderEngine = context->renderEngine;
    
    VectorList<RenderModel> models;
    models.reserve(contextModel.nodes.size());

    {
#ifdef PROFILE
        DevProfiler devProfiler("System- Make RenderModels", [](String value) {
            std::cout << value;
        });

        cout << "Profile: Nodes count: " << contextModel.nodes.size() << std::endl;
#endif

        // FUTURE: can this be done on parallel threads?
        for (auto& node : contextModel.nodes) {
            List<SP<SomeRenderer>> renderers;
            node->TypeComponents<SomeRenderer>(renderers);
            for (auto& renderer : renderers) {
                /// Some renderers use direct render (Example: imGui)
                renderer->RenderInto(contextModel);

                /// Most renderers produce render models, for a list of draw calls to be sent to the
                /// GPU by the graphics engine
                auto thisRenderModels = renderer->MakeRenderModels();
                auto thisRenderModelCount = thisRenderModels.size();

                GUARD_CONTINUE(thisRenderModelCount > 0)

                AddRange(models, thisRenderModels);
            }
        }
    }

    systemModel.models = models;

    auto& cameras = contextModel.cameras;

    // Execute a complete render pass for each camera
    // Example: Camera A renders moving content, camera B renders fixed UI
    // FUTURE: support limiting which nodes are rendered for each camera,
    // either by using frustrum culling or a filter
    // FUTURE: support occlusion filtering for nodes based on camera view
    for (auto& camera : cameras) {
        GUARD_CONTINUE(camera)

        renderEngine->RenderStart(contextModel);
        camera->PreRender(contextModel);

#ifdef PROFILE
        DevProfiler devProfilerRenderCamera("Render- Camera", [](String value) {
            cout << value;
        });
#endif

        for (auto& processor : processors) {
            GUARD_CONTINUE(processor->IsActive())
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
