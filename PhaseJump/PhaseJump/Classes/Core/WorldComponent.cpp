#include "WorldComponent.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

WorldComponent::WorldComponent(String name) :
    attachmentCore(*this) {
    attachmentCore.onAddSignalHandlerFunc = [this](auto& core, auto& signalHandler) {
        GUARD(owner)
        owner->SetIsListener(true);
    };

    PlanUIFunc planUIFunc = [this](auto args) {
        args.planner.Text([this]() {
            VectorList<String> signalNames = attachmentCore.SignalHandlerNames();
            auto text = Joined(signalNames, ", ");

            return UIPlanner::TextConfig{ .label = "Signals", .text = text };
        });

        args.planner.InputBool({ .label = "Is Enabled",
                                 .binding = { [this]() { return IsEnabled(); },
                                              [this](auto& value) { Enable(value); } } });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

// FUTURE: virtual void UpdateFromSerializedProperties(bool forceUpdate) {}

UP<UIPlan> WorldComponent::MakeUIPlan(String context) {
    try {
        auto& planUIFunc = planUIFuncs.at(context);
        auto result = NEW<UIPlan>();

        UIPlanner planner(*result.get());
        planUIFunc({ .component = *this, .context = context, .planner = planner });

        return result;
    } catch (...) {
        return {};
    }
}

bool WorldComponent::IsEnabled() const {
    GUARDR(owner, {})
    GUARDR(owner->IsEnabled(), {})

    return attachmentCore.IsEnabled();
}

SomeWorldComponent& WorldComponent::Enable(bool value) {
    attachmentCore.Enable(value);
    return *this;
}

SomeWorldComponent& WorldComponent::AddSignalHandler(SignalHandler handler) {
    attachmentCore.AddSignalHandler(handler);
    return *this;
}
