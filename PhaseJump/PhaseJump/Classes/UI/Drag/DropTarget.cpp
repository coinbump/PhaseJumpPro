#include "DropTarget.h"
#include "UIWorldSystem.h"

using namespace std;
using namespace PJ;

DropTarget::DropTarget() {
    AddSignalHandler<DragEnterUIEvent>({ .id = SignalId::DragEnter,
                                         .func = [this](auto& owner, auto& event) {
                                             OnDragEnter(event.dragModel);
                                         } });

    AddSignalHandler({ .id = SignalId::DragExit,
                       .func = [this](auto& owner, auto& signal) { OnDragExit(); } });
}

void DropTarget::SetState(StateType value) {
    GUARD(state != value)

    state = value;
    OnStateChange();
}

bool DropTarget::CanAcceptDrag(DragModel const& dragModel) {
    GUARDR(canAcceptDragFunc, true)
    return canAcceptDragFunc(*this, dragModel);
}

void DropTarget::OnDragEnter(DragModel const& dragModel) {
    // PJ::Log("OnDragEnter");
    SetState(CanAcceptDrag(dragModel) ? StateType::DragAccept : StateType::DragReject);
}

void DropTarget::OnDragExit() {
    // PJ::Log("OnDragExit");
    SetState(StateType::Default);
}

void DropTarget::OnDrop(DragModel const& dragModel) {
    GUARD(onDropFunc)
    onDropFunc(*this, dragModel);
}

void DropTarget::Awake() {
    Base::Awake();

    if (nullptr == owner->TypeComponent<Collider2D>()) {
        PJ::Log("ERROR. DropTarget requires a collider.");
    }

    OnStateChange();
}

void DropTarget::OnStateChange() {
    GUARD(onStateChangeFunc)
    onStateChangeFunc(*this);
}
