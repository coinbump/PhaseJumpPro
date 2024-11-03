#include "SomeDropTarget.h"
#include "UIWorldSystem.h"

using namespace std;
using namespace PJ;

SomeDropTarget::SomeDropTarget() {
    signalFuncs[SignalId::DragEnter] = [](auto& owner, auto& signal) {
        auto& event = *(static_cast<DragEnterUIEvent const*>(&signal));
        static_cast<This*>(&owner)->OnDragEnter(event.dragModel);
    };

    signalFuncs[SignalId::DragExit] = [](auto& owner, auto& signal) {
        static_cast<This*>(&owner)->OnDragExit();
    };
}

void SomeDropTarget::SetState(StateType value) {
    GUARD(state != value)

    state = value;
    OnStateChange();
}

bool SomeDropTarget::CanAcceptDrag(DragModel const& dragModel) {
    GUARDR(canAcceptDragFunc, true)
    return canAcceptDragFunc(*this, dragModel);
}

void SomeDropTarget::OnDragEnter(DragModel const& dragModel) {
    // PJ::Log("OnDragEnter");
    SetState(CanAcceptDrag(dragModel) ? StateType::DragAccept : StateType::DragReject);
}

void SomeDropTarget::OnDragExit() {
    // PJ::Log("OnDragExit");
    SetState(StateType::Default);
}

void SomeDropTarget::OnDrop(DragModel const& dragModel) {
    GUARD(onDropFunc)
    onDropFunc(*this, dragModel);
}

void SomeDropTarget::Awake() {
    Base::Awake();

    if (IsLogVerbose()) {
        if (nullptr == owner->TypeComponent<SomeCollider2D>()) {
            PJ::Log("ERROR. DropTarget requires a collider.");
        }
    }

    OnStateChange();
}

void SomeDropTarget::OnStateChange() {
    GUARD(onStateChangeFunc)
    onStateChangeFunc(*this);
}
