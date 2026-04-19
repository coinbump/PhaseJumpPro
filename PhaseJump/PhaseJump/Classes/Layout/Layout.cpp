#include "Layout.h"
#include "Funcs.h"
#include "WorldComponent.h"

using namespace std;
using namespace PJ;

void Layout::Awake(WorldComponent& component) {
    updatable.onUpdateFunc = [this](Updatable&, TimeSlice) {
        LayoutIfNeeded();
        return FinishType::Continue;
    };

    component.GetUpdatable().onUpdateFunc = [this](Updatable&, TimeSlice time) {
        return updatable.OnUpdate(time);
    };

    component.AddSignalHandler({ .id = SignalId::ChildNodeAdd,
                                 .func = [this](auto& _component, auto& event) {
                                     SetNeedsLayout();
                                 } });
    component.AddSignalHandler({ .id = SignalId::ChildNodeRemove,
                                 .func = [this](auto& _component, auto& event) {
                                     SetNeedsLayout();
                                 } });
}

void Layout::Start(WorldComponent&) {
    LayoutIfNeeded();
}
