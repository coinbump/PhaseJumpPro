#include "SomeLayout.h"
#include "Funcs.h"

using namespace std;
using namespace PJ;

void SomeLayout::Awake() {
    Base::Awake();

    Updatable::OnUpdateFunc onUpdateFunc = [=, this](Updatable& updatable, TimeSlice time) {
        LayoutIfNeeded();
        return FinishType::Continue;
    };
    GetUpdatable().onUpdateFunc = onUpdateFunc;

    AddSignalHandler({ .id = SignalId::AddChildNode,
                       .func = [this](auto& _component, auto& event) { SetNeedsLayout(); } });
    AddSignalHandler({ .id = SignalId::RemoveChildNode,
                       .func = [this](auto& _component, auto& event) { SetNeedsLayout(); } });
}

void SomeLayout::Start() {
    Base::Start();

    LayoutIfNeeded();
}
