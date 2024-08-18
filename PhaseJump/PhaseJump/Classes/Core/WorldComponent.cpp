#include "WorldComponent.h"
#include "Matrix4x4.h"
#include "World.h"
#include "WorldNode.h"
#include <TSMatrix4D.h>

using namespace PJ;

LocalPosition PJ::ScreenToLocal(SomeWorldComponent& component, ScreenPosition screenPos) {
    LocalPosition result;

    auto owner = component.owner;
    GUARDR(owner, result)

    auto world = owner->world;
    GUARDR(world, result)

    // Get the camera
    SP<SomeCamera> camera = world->MainCamera();
    GUARDR(camera, result)

    auto worldPosition = camera->ScreenToWorld(screenPos);

    auto worldModelMatrix = world->WorldModelMatrix(*owner);
    Terathon::Point3D point(worldPosition.x, worldPosition.y, worldPosition.z);
    auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

    result = LocalPosition(Vector3(localPosition.x, localPosition.y, 0));

    return result;
}

template <>
void WorldComponent<ComposeWorldComponentCore>::Awake() {
    this->core.awakeFuncs(*this);
}

template <>
void WorldComponent<ComposeWorldComponentCore>::Start() {
    this->core.startFuncs(*this);
}

template <>
void WorldComponent<ComposeWorldComponentCore>::LateUpdate() {
    this->core.lateUpdateFuncs(*this);
}

template <>
void WorldComponent<ComposeWorldComponentCore>::OnUpdate(TimeSlice time) {
    this->core.updateFuncs(*this, time);
}
