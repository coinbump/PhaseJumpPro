#include "WorldComponent.h"
#include "WorldNode.h"

using namespace PJ;

void WorldComponent::DestroyOwner(float afterSeconds)
{
    if (owner.expired()) { return; }
    owner.lock()->Destroy(afterSeconds);
}

std::shared_ptr<GeoTransform> WorldComponent::Transform() const
{
    if (owner.expired()) { return nullptr; }
    return owner.lock()->transform;
}
