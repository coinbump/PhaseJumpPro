#include "CameraCullRenderProcessor.h"
#include "Camera.h"
#include "CollectionUtils.h"
#include "RenderWorldSystem.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void CameraCullRenderProcessor::Process(RenderCameraModel& cameraModel) {
    GUARD(cameraModel.camera)

    // TODO: this is broken with new render DAG. Fix this
    /*
    auto beforeCullCount = cameraModel.renderModels.size();

    // TODO: remove from the DAG, not renderModels
    RemoveIf(cameraModel.renderModels, [&](MaterialRenderModel const& model) {
        auto mesh = model.GetMesh() * model.matrix;
        return cameraModel.camera->IsCulled(mesh);
    });

    auto afterCullCount = cameraModel.renderModels.size();
    if (beforeCullCount > afterCullCount) {
        PJ::Log(std::format("Culled: {}", beforeCullCount - afterCullCount));
    }
     */
}
