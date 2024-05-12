#include "SomeDragHandler.h"
#include "SomeCamera.h"
#include "Matrix4x4.h"
#include <TSMatrix4D.h>
#include "World.h"

using namespace std;
using namespace PJ;

bool SomeDragHandler::IsDragging() const
{
    return state == StateType::Drag;
}

void SomeDragHandler::OnDragEnd()
{
    state = StateType::Default;
}

void SomeDragHandler::StartDrag(WorldPosition inputPosition)
{
    // Allow drag to be handled by another object
    if (!dragTarget.expired())
    {
        dragTarget.lock()->StartDrag(inputPosition);
        return;
    }

    state = StateType::Drag;

    dragStartPosition = owner.lock()->transform->WorldPosition();
    dragStartInputPosition = inputPosition;

    auto uiSystem = UISystem();
    if (nullptr == uiSystem) { return; }

    auto dragModel = MAKE<DragModel>(SCAST<SomeDragHandler>(shared_from_this()));
    uiSystem->StartDrag(dragModel);

    OnDragStart(inputPosition);
}

void SomeDragHandler::OnPointerDownEvent(PointerDownUIEvent<LocalPosition> event) {
    if (owner.expired()) { return; }
    //Debug.Log("DragHandler2D OnPointerDown at: " + eventData.pressPosition.ToString());

    auto inputLocalPosition = event.pressPosition;
    auto worldModelMatrix = World()->WorldModelMatrix(*owner.lock());
    auto inputWorldPosition = worldModelMatrix.MultiplyPoint(inputLocalPosition);
    StartDrag(inputWorldPosition);
}
