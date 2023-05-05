#include "SomeWorldComponent.h"
#include "WorldNode.h"

using namespace PJ;

void SomeWorldComponent::DestroyOwner(float afterSeconds)
{
    if (owner.expired()) { return; }
    owner.lock()->Destroy(afterSeconds);
}

SP<GeoTransform> SomeWorldComponent::Transform() const
{
    if (owner.expired()) { return nullptr; }
    return owner.lock()->transform;
}

SP<WorldNode> SomeWorldComponent::Node() const {
    if (owner.expired()) { return nullptr; }
    auto owner = this->owner.lock();
    auto ownerNode = SCAST<WorldNode>(owner);
    return ownerNode;
}
