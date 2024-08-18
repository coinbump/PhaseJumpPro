#include "SomeWorldComponent.h"
#include "Matrix4x4.h"
#include "World.h"
#include "WorldNode.h"

using namespace PJ;

Matrix4x4 SomeWorldComponent::ModelMatrix() const {
    Matrix4x4 result;
    result.LoadIdentity();

    GUARDR(owner && owner->world, result)
    return owner->world->WorldModelMatrix(*owner);
}

void SomeWorldComponent::CheckedAwake() {
    life.CheckedAwake([this]() { this->Awake(); });
}

void SomeWorldComponent::CheckedStart() {
    life.CheckedStart([this]() { this->Start(); });
}
