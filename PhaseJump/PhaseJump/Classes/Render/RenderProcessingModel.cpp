#include "RenderProcessingModel.h"
#include "RenderProcessor.h"

using namespace std;
using namespace PJ;

// TODO: doesn't make sense. The cameraModel has the phase in it
void RenderProcessingModel::ProcessPhase(RenderCameraModel& cameraModel, String phase) {
    for (auto& processor : Processors()) {
        GUARD_CONTINUE(processor->IsEnabled())

        if (processor->phases.size() == 0) {
            PJ::Log("ERROR. Render processor isn't registered for a render phase.");
            continue;
        }

        GUARD_CONTINUE(processor->phases.contains(phase));
        processor->Process(cameraModel);
    }
}

void RenderProcessingModel::ProcessPhase(String phase) {
    for (auto& processor : Processors()) {
        GUARD_CONTINUE(processor->IsEnabled())

        if (processor->phases.size() == 0) {
            PJ::Log("ERROR. Render processor isn't registered for a render phase.");
            continue;
        }

        GUARD_CONTINUE(processor->phases.contains(phase));
        processor->Process(phase);
    }
}
