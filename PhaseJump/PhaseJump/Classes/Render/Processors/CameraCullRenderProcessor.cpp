#include "CameraCullRenderProcessor.h"
#include "CollectionUtils.h"
#include "RenderWorldSystem.h"
#include "SomeCamera.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void CameraCullRenderProcessor::Process(RenderSystemModel& systemModel) {
    GUARD(systemModel.camera)

    auto beforeCullCount = systemModel.models.size();

    RemoveIf(systemModel.models, [&](RenderModel const& model) {
        Mesh mesh = model.mesh * model.matrix;
        return systemModel.camera->IsCulled(mesh);
    });

    auto afterCullCount = systemModel.models.size();
    if (beforeCullCount > afterCullCount) {
        cout << "Culled: " << beforeCullCount - afterCullCount << std::endl;
    }
}
