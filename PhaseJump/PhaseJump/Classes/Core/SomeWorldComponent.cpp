#include "SomeWorldComponent.h"
#include "Matrix4x4.h"
#include "UIPlanner.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

using This = SomeWorldComponent;

SomeWorldComponent::SomeWorldComponent(String name) :
    _core(*this) {
    _core.name = name;

    PlanUIFunc planUIFunc = [this](auto& component, String context, UIPlanner& planner) {
        planner.Text([this]() {
            VectorList<String> signalNames = _core.SignalHandlerNames();
            auto text = Joined(signalNames, ", ");

            return UIPlanner::TextConfig{ .label = "Signals", .text = text };
        });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

This& SomeWorldComponent::Enable(bool value) {
    GUARDR(isEnabled != value, *this)
    isEnabled = value;

    if (onEnabledChangeFunc) {
        onEnabledChangeFunc(*this);
    }
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
        planUIFunc(*this, context, planner);

        return result;
    } catch (...) {
        return {};
    }
}

void SomeWorldComponent::Awake() {
    GUARD(awakeFunc)
    awakeFunc(*this);
}

void SomeWorldComponent::Start() {
    GUARD(startFunc)
    startFunc(*this);
}

This& SomeWorldComponent::AddSignalHandler(SignalHandler handler) {
    _core.AddSignalHandler(handler);
    OnAddSignalHandler();
    return *this;
}

void SomeWorldComponent::OnAddSignalHandler() {
    GUARD(owner)
    owner->isListener = true;
}
