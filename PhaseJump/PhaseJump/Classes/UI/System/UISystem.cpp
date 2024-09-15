#include "UISystem.h"
#include "Dev.h"
#include "World.h"

using namespace std;
using namespace PJ;

UISystem* UISystem::shared;

SP<SomeCamera> UISystem::Camera() const {
    return World()->MainCamera();
}

void UISystem::ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) {
    Base::ProcessUIEvents(uiEvents);
}

void UISystem::Awake() {
    Base::Awake();

    if (nullptr != shared) {
        PJ::Log("ERROR. Only 1 UISystem supported");
        return;
    }
    shared = this;
}

void UISystem::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    OnDragUpdate();
}
