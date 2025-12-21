#include "Layout.h"
#include "Funcs.h"

using namespace std;
using namespace PJ;

void Layout::Awake() {
    Base::Awake();

    Updatable::OnUpdateFunc onUpdateFunc = [=, this](Updatable& updatable, TimeSlice time) {
        LayoutIfNeeded();
        return FinishType::Continue;
    };
    GetUpdatable().onUpdateFunc = onUpdateFunc;

    AddSignalHandler({ .id = SignalId::ChildNodeAdd,
                       .func = [this](auto& _component, auto& event) { SetNeedsLayout(); } });
    AddSignalHandler({ .id = SignalId::ChildNodeRemove,
                       .func = [this](auto& _component, auto& event) { SetNeedsLayout(); } });
}

void Layout::Start() {
    Base::Start();

    LayoutIfNeeded();
}
