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

    uiSystemResolver = [this]() -> UIWorldSystem* {
        GUARDR(owner, {})

        auto const world = owner->World();
        GUARDR(world, {})

        return world->TypeSystem<UIWorldSystem>();
    };
}

bool DragHandler::IsDragging() const {
    return state == StateType::Began;
}

void DragHandler::Cancel() {
    switch (state) {
    case StateType::Began:
        {
            auto uiSystem = UISystem();
            GUARD(uiSystem)

            uiSystem->CancelDrag();
            break;
        }
    default:
        break;
    }
}

void DragHandler::OnDragCancel() {
    state = StateType::Default;
}

void DragHandler::OnDragEnd(WorldPosition position) {
    state = StateType::Default;
}

void DragHandler::StartDrag(WorldPosition inputPosition) {
    GUARD(owner)

    // Allow drag to be handled by another object, the state for this handler does not update
    DragHandler* dragTarget{};
    if (dragTargetResolver) {
        dragTarget = dragTargetResolver();
    }
    if (dragTarget) {
        dragTarget->StartDrag(inputPosition);
        return;
    }

    state = StateType::Began;

    dragNodeStartPosition = owner->transform.WorldPosition();
    dragStartPosition = inputPosition;

    auto uiSystem = UISystem();
    GUARD(uiSystem)

    auto dragModel = MAKE<DragModel>(SCAST<DragHandler>(shared_from_this()));
    uiSystem->StartDrag(dragModel);

    OnDragStart(inputPosition);
}

void DragHandler::OnPointerDown(PointerDownUIEvent const& event) {
    GUARD(owner)

    auto inputWorldPosition = ScreenToWorld(*this, event.core.screenPos);
    StartDrag(inputWorldPosition);
}
