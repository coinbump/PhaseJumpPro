#include "UISystem.h"

using namespace std;
using namespace PJ;

SP<UISystem> UISystem::shared;

void UISystem::ProcessUIEvents(VectorList<SP<SomeUIEvent>> const& uiEvents) {
    Base::ProcessUIEvents(uiEvents);
}

void UISystem::Awake() {
    Base::Awake();

    if (nullptr != shared) {
        PJLog("ERROR. Only 1 UISystem supported")
        return;
    }
    shared = SCAST<UISystem>(shared_from_this());
}

void UISystem::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    OnDragUpdate();
}
