#include "CameraCullRenderProcessor.h"
#include "CollectionUtils.h"
#include "RenderWorldSystem.h"
#include "SomeCamera.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void CameraCullRenderProcessor::Process(CameraRenderModel& cameraModel) {
    GUARD(cameraModel.camera)

    auto beforeCullCount = cameraModel.models.size();

    RemoveIf(cameraModel.models, [&](RenderModel const& model) {
        Mesh mesh = model.mesh * model.matrix;
        return cameraModel.camera->IsCulled(mesh);
    });

    auto afterCullCount = cameraModel.models.size();
    if (beforeCullCount > afterCullCount) {
        cout << "Culled: " << beforeCullCount - afterCullCount << std::endl;
    }
}
