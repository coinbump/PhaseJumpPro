#include "SomeWorldComponent.h"
#include "Matrix4x4.h"
#include "UIPlanner.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

using This = SomeWorldComponent;

SomeWorldComponent::SomeWorldComponent(String name) :
    attachmentCore(*this) {
    attachmentCore.name = name;

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

This& SomeWorldComponent::Enable(bool value) {
    GUARDR(isEnabled != value, *this)
    isEnabled = value;

    OnEnabledChange();
    return *this;
}

Matrix4x4 SomeWorldComponent::ModelMatrix() const {
    Matrix4x4 result;
    result.LoadIdentity();

    GUARDR(owner, result)
    return owner->ModelMatrix();
}

void SomeWorldComponent::CheckedAwake() {
    life.CheckedAwake([this]() { this->Awake(); });
}

void SomeWorldComponent::CheckedStart() {
    life.CheckedStart([this]() { this->Start(); });
}

Vector3 SomeWorldComponent::LocalToWorld(Vector3 localPos) const {
    GUARDR(owner, localPos)
    return owner->LocalToWorld(localPos);
}

Vector3 SomeWorldComponent::WorldToLocal(Vector3 worldPos) const {
    GUARDR(owner, worldPos)
    return owner->WorldToLocal(worldPos);
}

UP<UIPlan> SomeWorldComponent::MakeUIPlan(String context) {
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

This& SomeWorldComponent::AddSignalHandler(SignalHandler handler) {
    attachmentCore.AddSignalHandler(handler);
    OnAddSignalHandler();
    return *this;
}

void SomeWorldComponent::OnAddSignalHandler() {
    GUARD(owner)
    owner->isListener = true;
}

bool SomeWorldComponent::IsEnabled() const {
    GUARDR(owner, {})
    GUARDR(owner->IsEnabled(), {})

    return isEnabled;
}
