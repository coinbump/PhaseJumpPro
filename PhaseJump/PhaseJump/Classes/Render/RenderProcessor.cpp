#include "RenderProcessor.h"
#include "RenderCameraModel.h"
#include "RenderPassModel.h"

using namespace PJ;

void RenderProcessor::Process(SomeRenderModelNode& rootNode) {}

bool RenderProcessor::Phase::IsCamera() const {
    GUARDR(rootNode, false)
    return dynamic_cast<RenderCameraModel*>(rootNode->core.get()) != nullptr;
}
