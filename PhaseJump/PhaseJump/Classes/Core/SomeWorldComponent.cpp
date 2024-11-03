#include "SomeWorldComponent.h"
#include "Matrix4x4.h"
#include "UIPlanner.h"
#include "World.h"
#include "WorldNode.h"

using namespace PJ;

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

Vector3 SomeWorldComponent::LocalToWorld(Vector3 localPos) {
    GUARDR(owner, localPos)
    return owner->LocalToWorld(localPos);
}

Vector3 SomeWorldComponent::WorldToLocal(Vector3 worldPos) {
    GUARDR(owner, worldPos)
    return owner->WorldToLocal(worldPos);
}

void SomeWorldComponent::Signal(String id, SomeSignal const& signal) {
    auto i = signalFuncs.find(id);
    if (i != signalFuncs.end()) {
        i->second(*this, signal);
    }
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
