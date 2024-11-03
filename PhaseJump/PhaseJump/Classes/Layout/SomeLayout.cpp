#include "SomeLayout.h"
#include "Funcs.h"

using namespace std;
using namespace PJ;

void SomeLayout::Awake() {
    Base::Awake();

    Func applyLayoutAction = [](auto& _component) {
        auto component = static_cast<This*>(&_component);
        GUARD(component->autoApply)
        component->LayoutIfNeeded();
    };
    Override(startFunc, applyLayoutAction);

    Updatable::OnUpdateFunc onUpdateFunc = [=, this](Updatable& updatable, TimeSlice time) {
        applyLayoutAction(*this);
        return FinishType::Continue;
    };
    updatable.onUpdateFunc = onUpdateFunc;

    signalFuncs[SignalId::AddChildNode] = [](auto& _component, auto& event) {
        auto component = static_cast<This*>(&_component);
        component->SetNeedsLayout();
    };
    signalFuncs[SignalId::RemoveChildNode] = [](auto& _component, auto& event) {
        auto component = static_cast<This*>(&_component);
        component->SetNeedsLayout();
    };
}
