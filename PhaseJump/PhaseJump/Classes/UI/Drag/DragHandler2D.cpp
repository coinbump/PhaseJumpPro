#include "DragHandler2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void DragHandler2D::Drop(DropType dropType) {
    GUARD(onDropFunc)
    onDropFunc(*this, dropType);
}

void DragHandler2D::InOnDropSnapBack() {
    onDropFunc = [](auto& dragHandler, auto dropType) {
        GUARD(dragHandler.owner)
        dragHandler.owner->transform.SetWorldPosition(dragHandler.dragNodeStartPosition);
    };
}

void DragHandler2D::OnDragUpdate(WorldPosition position) {
    GUARD(owner)
    NodeTransform& transform = owner->transform;

    auto internalOffset = dragNodeStartPosition - dragStartPosition;
    transform.SetWorldPositionXY(position + internalOffset);

    GUARD(onDragUpdateFunc)
    onDragUpdateFunc(*this, position);
}

void DragHandler2D::OnDragEnd(WorldPosition position) {
    Base::OnDragEnd(position);

    Drop(DropType::End);
}

void DragHandler2D::OnDragCancel() {
    Base::OnDragCancel();

    Drop(DropType::Cancel);
}
