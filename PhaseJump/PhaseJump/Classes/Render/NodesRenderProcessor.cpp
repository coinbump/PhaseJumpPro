#include "NodesRenderProcessor.h"
#include "NodeHandler.h"
#include "RenderCameraModel.h"

using namespace std;
using namespace PJ;

NodesRenderProcessor::NodesRenderProcessor(Config config, RenderProcessor::Config baseConfig) :
    Base(baseConfig),
    type(config.type) {
    processFunc = [this](auto& phase) {
        GUARD(phase.cameraModel)

        for (auto& node : phase.cameraModel->nodes) {
            for (auto& component : node->Components()) {
                auto nodeHandlerPtr = dynamic_cast<NodeHandler*>(component.get());
                GUARD_CONTINUE(nodeHandlerPtr)

                // If a type is specified, make sure they match
                if (!IsEmpty(type)) {
                    GUARD_CONTINUE(type == nodeHandlerPtr->type);
                }

                auto& nodeHandler = *nodeHandlerPtr;
                nodeHandler.Run(phase);
            }
        }
    };
}
