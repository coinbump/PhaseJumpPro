#include "UISystem.h"
#include "SomeDragHandler.h"
#include "SomeCamera.h"

using namespace std;
using namespace PJ;

void UISystem::OnDragUpdate()
{
    if (!IsDragging()) { return; }
    if (nullptr == mouseDevice || !mouseDevice->IsAvailable()) { return; }

    auto isMouseButtonDown = mouseDevice->IsButtonDown(PointerInputButton::Left);

    if (isMouseButtonDown || dragState == DragState::LockDragMouseUp)
    {
        if (nullptr == dragModel) { return; }

        auto dragged = dragModel->dragHandler;
        if (nullptr == dragged) { return; }

        CheckDropTargets();

        auto inputScreenPosition = mouseDevice->Position();
        auto inputWorldPosition = PJ::WorldPosition(Camera()->ScreenToWorld(Vector3(inputScreenPosition)));
        dragged->OnDragUpdate(WorldPosition(inputWorldPosition));
    }
    else
    {
        OnDragEnd();
    }
}

bool UISystem::ProcessPointerDownEvent(SP<PointerDownUIEvent<ScreenPosition>> pointerDownEvent) {
    if (nullptr == pointerDownEvent) { return false; }

    if (lockDrag)
    {
        switch (dragState)
        {
            case DragState::LockDragMouseDown:
                break;
            case DragState::LockDragMouseUp:
                OnDragEnd();
                return true;
            default:
                break;
        }
    }

    return Base::ProcessPointerDownEvent(pointerDownEvent);
}

bool UISystem::ProcessPointerUpEvent(SP<PointerUpUIEvent> pointerUpEvent) {
    if (nullptr == pointerUpEvent) { return false; }

    if (lockDrag)
    {
        switch (dragState)
        {
            case DragState::LockDragMouseDown:
                dragState = DragState::LockDragMouseUp;
                return true;
            case DragState::LockDragMouseUp:
                break;
            default:
                break;
        }
    }

    return Base::ProcessPointerUpEvent(pointerUpEvent);
}

void UISystem::CheckDropTargets()
{
    // FUTURE: support drop targets (from C#)
    //Debug.Log("Drop: Position: " + mouseDevice->position.ToString());
//    auto worldPosition = mouseDevice->WorldPosition;
//    if (null == worldPosition) { return; }
//
//    auto raycastHits = Physics2D.RaycastAll(worldPosition, Vector2.zero);
//    SomeDropTarget hitBehavior = null;
//
//    //Debug.Log("Drop: RaycastHits: " + raycastHits.ToString());
//
//    auto dragItems = DraggedItems;
//
//    foreach (RaycastHit2D raycastHit in raycastHits)
//    {
//        if (raycastHit.collider.gameObject == dragModel.dragHandler.gameObject)
//        {
//            continue;
//        }
//
//        if (raycastHit.collider != null)
//        {
//            hitBehavior = raycastHit.collider.gameObject.GetComponent<SomeDropTarget>();
//        }
//
//        if (null != hitBehavior && hitBehavior.CanAcceptDrag(dragItems))
//        {
//            break;
//        }
//    }
//
//    bool isAlreadyIn = false;
//    if (null != dropTargetOverObject && dropTargetOverObject.TryGetTarget(out WorldNode dropTargetTarget))
//    {
//        auto activeDropTarget = dropTargetTarget.GetComponent<SomeDropTarget>();
//        if (activeDropTarget != hitBehavior)
//        {
//            activeDropTarget.OnDragLeave();
//        }
//        else
//        {
//            isAlreadyIn = true;
//        }
//    }
//
//    if (null == hitBehavior)
//    {
//        dropTargetOverObject = null;
//        return;
//    }
//
//    if (!isAlreadyIn)
//    {
//        dropTargetOverObject = new WP<WorldNode>(hitBehavior.gameObject);
//        hitBehavior.OnDragEnter(dragItems);
//    }
}

void UISystem::StartDrag(SP<DragModel> dragModel)
{
    // A mouse button down will start a drag but we don't want that if the drag is locked
    switch (dragState)
    {
        case DragState::LockDragMouseUp:
            //Debug.Log("Drag is locked. Cancel the current drag first.");
            return;
        default:
            break;
    }

    OnDragEnd();
    this->dragModel = dragModel;

    dragState = lockDrag ? DragState::LockDragMouseDown : DragState::Drag;
}

void UISystem::OnDragEnd()
{
    // FUTURE: support drop targets (from C#)
//    if (null != dropTargetOverObject && dropTargetOverObject.TryGetTarget(out WorldNode dropTargetTarget))
//    {
//        auto dropTarget = dropTargetTarget.GetComponent<SomeDropTarget>();
//        dropTarget.OnDragLeave();
//
//        if (dropTarget.CanAcceptDrag(DraggedItems))
//        {
//            dropTarget.OnAcceptDrag(DraggedItems);
//        }
//    }
//    dropTargetOverObject = null;

    if (dragModel) {
        dragModel->dragHandler->OnDragEnd();
        dragModel.reset();
    }
    dragState = DragState::Default;
}
