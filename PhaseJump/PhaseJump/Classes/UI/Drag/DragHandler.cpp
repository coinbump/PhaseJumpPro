#include "DragHandler.h"
#include "Camera.h"
#include "Matrix4x4.h"
#include "World.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

DragHandler::DragHandler() {
    AddSignalHandler<PointerDownUIEvent>({ .id = SignalId::PointerDown,
                                           .func = [](auto& component, auto& event) {
                                               static_cast<This*>(&component)->OnPointerDown(event);
                                           } });

    uiSystemResolver = [this]() { return owner->World()->TypeSystem<UIWorldSystem>(); };
}

bool DragHandler::IsDragging() const {
    return state == StateType::Drag;
}

void DragHandler::OnDragEnd() {
    state = StateType::Default;
}

void DragHandler::StartDrag(WorldPosition inputPosition) {
    GUARD(owner)

    // Allow drag to be handled by another object
    if (!dragTarget.expired()) {
        dragTarget.lock()->StartDrag(inputPosition);
        return;
    }

    state = StateType::Drag;

    dragStartPosition = owner->transform.WorldPosition();
    dragStartInputPosition = inputPosition;

    auto uiSystem = UISystem();
    GUARD(uiSystem)

    auto dragModel = MAKE<DragModel>(SCAST<DragHandler>(shared_from_this()));
    uiSystem->StartDrag(dragModel);

    OnDragStart(inputPosition);
}

void DragHandler::OnPointerDown(PointerDownUIEvent const& event) {
    GUARD(owner)

    // Debug.Log("DragHandler2D OnPointerDown at: " +
    // eventData.pressPosition.ToString());

    auto inputWorldPosition = ScreenToWorld(*this, event.core.screenPos);
    StartDrag(inputWorldPosition);
}
