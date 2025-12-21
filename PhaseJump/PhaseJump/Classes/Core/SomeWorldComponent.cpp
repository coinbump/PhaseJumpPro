#include "SomeWorldComponent.h"
#include "Matrix4x4.h"
#include "UIPlanner.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

using This = SomeWorldComponent;

Matrix4x4 SomeWorldComponent::ModelMatrix() const {
    Matrix4x4 result;
    result.LoadIdentity();

    auto owner = Node();
    GUARDR(owner, result)
    return owner->ModelMatrix();
}

Vector3 SomeWorldComponent::LocalToWorld(Vector3 localPos) const {
    auto owner = Node();
    GUARDR(owner, localPos)
    return owner->LocalToWorld(localPos);
}

Vector3 SomeWorldComponent::WorldToLocal(Vector3 worldPos) const {
    auto owner = Node();
    GUARDR(owner, worldPos)
    return owner->WorldToLocal(worldPos);
}
