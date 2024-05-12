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
            Transform()->SetWorldPosition(dragStartPosition);
            break;
    }
}

void DragHandler2D::OnDragUpdate(WorldPosition inputPosition)
{
    auto internalOffset = dragStartPosition - dragStartInputPosition;
    Transform()->SetWorldPositionXY(inputPosition + internalOffset);

    // PJLog("Internal Offset: " + internalOffset.ToString() + " transform: " + transform.ToString());
}

void DragHandler2D::OnDragEnd()
{
    Base::OnDragEnd();

    Drop();
}
