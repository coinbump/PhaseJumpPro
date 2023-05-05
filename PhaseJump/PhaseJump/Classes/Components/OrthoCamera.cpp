#include "OrthoCamera.h"
#include "World.h"

using namespace std;
using namespace PJ;

// Note: this assumes the ortho camera matches the screen exactly
// FUTURE: support ortho camera size
Vector2 OrthoCamera::WorldToScreen(Vector3 position) {
    Vector2 result;

    if (owner.expired()) { return result; }
    auto owner = this->owner.lock();

    auto world = owner->World();
    auto renderContext = world.lock()->renderContext;
    if (nullptr == renderContext) {
        return result;
    }

    auto size = renderContext->Size();
    auto cameraPosition = owner->transform->position;

    Vector2 screenPosition(position.x - cameraPosition.x + size.x / 2.0f,
                           cameraPosition.y + size.y / 2.0f - position.y);
    return screenPosition;
}

// Note: this assumes the ortho camera matches the screen exactly
// FUTURE: support ortho camera size
Vector3 OrthoCamera::ScreenToWorld(Vector2 position) {
    Vector3 result;

    if (owner.expired()) { return result; }
    auto owner = this->owner.lock();

    auto world = owner->World();
    auto renderContext = world.lock()->renderContext;
    if (nullptr == renderContext) {
        return result;
    }

    auto size = renderContext->Size();
    auto cameraPosition = owner->transform->position;

    Vector3 worldPosition(position.x + cameraPosition.x - size.x / 2.0f,
                          size.y / 2.0f - position.y + cameraPosition.y,
                          0);
    return worldPosition;
}
