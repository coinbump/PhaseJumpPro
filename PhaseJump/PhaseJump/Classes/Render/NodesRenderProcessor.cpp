#include "NodesRenderProcessor.h"
#include "NodeHandler.h"
#include "RenderCameraModel.h"

using namespace std;
using namespace PJ;

NodesRenderProcessor::NodesRenderProcessor(
    Config const& config, RenderProcessor::Config baseConfig
) :
    Base(baseConfig),
    signalId(config.signalId) {
    processFunc = [this](auto& phase) {
        GUARD(phase.cameraModel)

        for (auto& node : phase.cameraModel->nodes) {
            for (auto& component : node->Components()) {
                component->Signal(signalId, NodesRenderSignal({ .phase = phase }));
            }
        }
    };
}
