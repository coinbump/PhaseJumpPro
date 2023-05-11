#include "DragHandler2D.h"

using namespace std;
using namespace PJ;

void DragHandler2D::Drop()
{
    switch (dropType)
    {
        case DropType::Stay:
            break;
        case DropType::SnapBack:
            Transform()->SetWorldPosition(dragStartPosition.position);
            break;
    }
}

void DragHandler2D::OnDragUpdate(WorldPosition inputPosition)
{
    auto internalOffset = dragStartPosition.position - dragStartInputPosition.position;
    Transform()->SetWorldPositionXY(inputPosition.position + internalOffset);

    // PJLog("Internal Offset: " + internalOffset.ToString() + " transform.position: " + transform.position.ToString());
}

void DragHandler2D::OnDragEnd()
{
    Base::OnDragEnd();

    Drop();
}
