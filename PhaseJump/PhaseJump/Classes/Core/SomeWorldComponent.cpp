#include "SomeWorldComponent.h"
#include "Matrix4x4.h"
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
