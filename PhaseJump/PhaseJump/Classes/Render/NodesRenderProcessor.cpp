#include "NodesRenderProcessor.h"
#include "NodeHandler.h"
#include "RenderCameraModel.h"
#include "RenderPassModel.h"

using namespace std;
using namespace PJ;

NodesRenderProcessor::NodesRenderProcessor(
    Config const& config, RenderProcessor::Config baseConfig
) :
    Base(baseConfig),
    signalId(config.signalId) {
    processFunc = [this](auto& phase) {
        GUARD(phase.rootNode)

        auto cameraModel = dynamic_cast<RenderCameraModel*>(phase.rootNode->core.get());
        GUARD(cameraModel)

        for (auto& node : cameraModel->nodes) {
            node->Signal(signalId, NodesRenderSignal({ .phase = phase }));
        }
    };
}
