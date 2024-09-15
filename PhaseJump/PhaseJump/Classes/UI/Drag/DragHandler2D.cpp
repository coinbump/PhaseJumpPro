#include "DragHandler2D.h"

using namespace std;
using namespace PJ;

void DragHandler2D::Drop() {
    GUARD(owner)
    NodeTransform& transform = owner->transform;

    switch (dropType) {
    case DropType::Stay:
        break;
    case DropType::SnapBack:
        transform.SetWorldPosition(dragStartPosition);
        break;
    }
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
