#include "RenderProcessingModel.h"
#include "RenderProcessor.h"

using namespace std;
using namespace PJ;

void RenderProcessingModel::ProcessPhase(String phase, RenderCameraModel* cameraModel) {
    for (auto& processor : Processors()) {
        GUARD_CONTINUE(processor->IsEnabled())

        if (processor->phases.size() == 0) {
            PJ::Log("ERROR. Render processor isn't registered for a render phase.");
            continue;
        }

        GUARD_CONTINUE(processor->phases.contains(phase));
        processor->Process({ .id = phase, .cameraModel = cameraModel });
    }
}
