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
    updatable.onUpdateFunc = onUpdateFunc;

    signalFuncs[SignalId::AddChildNode] = [this](auto& _component, auto& event) {
        SetNeedsLayout();
    };
    signalFuncs[SignalId::RemoveChildNode] = [this](auto& _component, auto& event) {
        SetNeedsLayout();
    };
}

void SomeLayout::Start() {
    Base::Start();

    LayoutIfNeeded();
}
