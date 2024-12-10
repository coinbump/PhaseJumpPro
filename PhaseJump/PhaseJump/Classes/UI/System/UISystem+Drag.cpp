#include "SomeCamera.h"
#include "SomeDragHandler.h"
#include "SomeDropTarget.h"
#include "UIWorldSystem.h"
#include "World.h"

using namespace std;
using namespace PJ;

// TODO: move to mouse motion event?
void UIWorldSystem::OnDragUpdate() {
    if (!IsDragging()) {
        return;
    }
    if (nullptr == mouseDevice || !mouseDevice->IsAvailable()) {
        return;
    }

    auto isMouseButtonDown = mouseDevice->IsButtonDown(PointerInputButton::Left);

    if (isMouseButtonDown || dragState == DragState::LockDragMouseUp) {
        if (nullptr == dragModel) {
            return;
        }

        auto dragged = dragModel->DragHandler();
        GUARD(dragged)

        auto inputScreenPosition = mouseDevice->Position() * world->uiScale;
        auto inputWorldPosition =
            PJ::WorldPosition(Camera()->ScreenToWorld(Vector3(inputScreenPosition)));
        dragged->OnDragUpdate(WorldPosition(inputWorldPosition));
    } else {
        OnDragEnd();
    }
}

void UIWorldSystem::CheckDropTargets(ScreenPosition screenPos) {
    GUARD(dragModel)
    // PJ::Log("Drop: Position: " + mouseDevice->position.ToString());

    auto hits = TestScreenHit(screenPos);
    auto hit = hits.size() > 0 ? &hits[0] : nullptr;
    WorldNode* hitNode = hit ? &hit->node : nullptr;
    auto activeDropTarget = this->activeDropTarget.lock();

    if (activeDropTarget && hitNode != activeDropTarget.get()) {
        DragExitUIEvent event;
        DispatchEvent(*activeDropTarget, SignalId::DragExit, event);
    }

    if (hitNode) {
        activeDropTarget = SCAST<WorldNode>(hitNode->shared_from_this());
        this->activeDropTarget = activeDropTarget;

        DragEnterUIEvent event(*dragModel);
        DispatchEvent(*activeDropTarget, SignalId::DragEnter, event);
    } else {
        activeDropTarget.reset();
    }
}

void UIWorldSystem::StartDrag(SP<DragModel> dragModel) {
    // Don't let hover UI interfere with drag
    SetActiveHover(nullptr);

    // A mouse button down will start a drag but we don't want that if the drag
    // is locked
    switch (dragState) {
    case DragState::LockDragMouseUp:
        // Debug.Log("Drag is locked. Cancel the current drag first.");
        return;
    default:
        break;
    }

    OnDragEnd();
    this->dragModel = dragModel;

    dragState = lockDrag ? DragState::LockDragMouseDown : DragState::Drag;
}

void UIWorldSystem::OnDragEnd() {
    GUARD(dragModel && dragModel->DragHandler())

    // Let the drag handler finish before checking drop targets
    dragModel->DragHandler()->OnDragEnd();

    auto activeDropTarget = this->activeDropTarget.lock();
    if (activeDropTarget) {
        auto dropTarget = activeDropTarget->TypeComponent<SomeDropTarget>();
        if (dropTarget) {
            dropTarget->OnDrop(*dragModel);
            dropTarget->OnDragExit();
        }
        this->activeDropTarget.reset();
    }

    dragModel.reset();
    dragState = DragState::Default;
}
