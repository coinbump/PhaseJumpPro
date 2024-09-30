#include "DragHandler2D.h"

using namespace std;
using namespace PJ;

void DragHandler2D::Drop() {
    GUARD(onDropFunc)
    onDropFunc(*this);
}

void DragHandler2D::InOnDropSnapBack() {
    onDropFunc = [](auto& dragHandler) {
        GUARD(dragHandler.owner)
        dragHandler.owner->transform.SetWorldPosition(dragHandler.dragStartPosition);
    };
}

void DragHandler2D::OnDragUpdate(WorldPosition inputPosition) {
    GUARD(owner)
    NodeTransform& transform = owner->transform;

    auto internalOffset = dragStartPosition - dragStartInputPosition;
    transform.SetWorldPositionXY(inputPosition + internalOffset);

    GUARD(onDragUpdateFunc)
    onDragUpdateFunc(*this);
    // PJ::Log("Internal Offset: " + internalOffset.ToString() + " transform: " +
    // transform.ToString());
}

void DragHandler2D::OnDragEnd() {
    Base::OnDragEnd();

    Drop();
}
