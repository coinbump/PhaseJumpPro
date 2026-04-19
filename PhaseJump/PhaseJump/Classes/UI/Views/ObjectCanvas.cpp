#include "ObjectCanvas.h"
#include "CanvasTool.h"
#include "DragGestureHandler2D.h"
#include "QuickBuilder.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

ObjectCanvas::ObjectCanvas() {}

ObjectCanvas::~ObjectCanvas() {
    if (activeTool) {
        activeTool->canvas = nullptr;
    }
}

void ObjectCanvas::Awake() {
    Base::Awake();

    QB(*this->owner).RectCollider();

    dragHandler = owner->AddComponentPtr<DragGestureHandler2D>();
    dragHandler->onDragGestureUpdateFunc = [this](auto update) {
        GUARD(activeTool && activeTool->onDragGestureUpdateFunc)
        activeTool->onDragGestureUpdateFunc(update);
    };

    auto toolsNode = MAKE<WorldNode>(WorldNode::Config{ .name = "Tools" });
    owner->Add(toolsNode);
    this->toolsNode = toolsNode;
}

void ObjectCanvas::SetActiveTool(SP<CanvasTool> value) {
    if (activeTool) {
        activeTool->canvas = nullptr;
    }

    activeTool = value;

    if (activeTool) {
        activeTool->canvas = this;
    }
}
