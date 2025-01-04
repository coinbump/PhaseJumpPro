#include "OrderRenderProcessor.h"
#include "RenderWorldSystem.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void OrderRenderProcessor::Process(RenderCameraModel& cameraModel) {
    GUARD(sortFunc)

    // Sort the models
    std::sort(cameraModel.renderModels.begin(), cameraModel.renderModels.end(), sortFunc);

    // Paint the order
    for (uint32_t i = 0; i < cameraModel.renderModels.size(); i++) {
        cameraModel.renderModels[i].order = i;
    }
}
