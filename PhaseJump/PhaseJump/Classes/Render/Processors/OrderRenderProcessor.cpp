#include "OrderRenderProcessor.h"
#include "RenderWorldSystem.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void OrderRenderProcessor::Process(RenderSystemModel& systemModel) {
    GUARD(sortFunc)

    // Sort the models
    std::sort(systemModel.models.begin(), systemModel.models.end(), sortFunc);

    // Paint the order
    for (uint32_t i = 0; i < systemModel.models.size(); i++) {
        systemModel.models[i].order = i;
    }
}
