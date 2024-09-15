#include "SomeDragHandler.h"
#include "Matrix4x4.h"
#include "SomeCamera.h"
#include "World.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

SomeDragHandler::SomeDragHandler() {
    signalHandlers[SignalId::PointerDown] = [](auto& component, auto& signal) {
        auto event = static_cast<PointerDownUIEvent const*>(&signal);
        static_cast<This*>(&component)->OnPointerDown(*event);
    };
}

bool SomeDragHandler::IsDragging() const {
    return state == StateType::Drag;
}

void SomeDragHandler::OnDragEnd() {
    state = StateType::Default;
}

void SomeDragHandler::StartDrag(WorldPosition inputPosition) {
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
    if (nullptr == uiSystem) {
        return;
    }

    // TODO: do we need shared_from_this here?
    auto dragModel = MAKE<DragModel>(SCAST<SomeDragHandler>(shared_from_this()));
    uiSystem->StartDrag(dragModel);

    OnDragStart(inputPosition);
}

void SomeDragHandler::OnPointerDown(PointerDownUIEvent const& event) {
    GUARD(owner)

    // Debug.Log("DragHandler2D OnPointerDown at: " +
    // eventData.pressPosition.ToString());

    auto inputLocalPosition = ScreenToLocal(*this, event.screenPos);
    GUARD(Node()->World());
    auto worldModelMatrix = Node()->World()->WorldModelMatrix(*owner);
    auto inputWorldPosition = worldModelMatrix.MultiplyPoint(inputLocalPosition);
    StartDrag(inputWorldPosition);
}
