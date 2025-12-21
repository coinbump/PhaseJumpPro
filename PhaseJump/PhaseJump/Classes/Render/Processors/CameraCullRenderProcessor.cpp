#include "CameraCullRenderProcessor.h"
#include "Camera.h"
#include "CollectionUtils.h"
#include "RenderWorldSystem.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void CameraCullRenderProcessor::Process(RenderCameraModel& cameraModel) {
    GUARD(cameraModel.camera)

    auto beforeCullCount = cameraModel.renderModels.size();

    RemoveIf(cameraModel.renderModels, [&](RenderModel const& model) {
        auto mesh = model.GetMesh() * model.matrix;
        return cameraModel.camera->IsCulled(mesh);
    });

    auto afterCullCount = cameraModel.renderModels.size();
    if (beforeCullCount > afterCullCount) {
        cout << "Culled: " << beforeCullCount - afterCullCount << std::endl;
    }
}
