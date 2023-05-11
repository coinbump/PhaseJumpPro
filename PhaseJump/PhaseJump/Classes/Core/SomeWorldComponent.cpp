#include "SomeWorldComponent.h"
#include "WorldNode.h"
#include "World.h"

using namespace PJ;

void SomeWorldComponent::DestroyOwner(float afterSeconds)
{
    if (owner.expired()) { return; }
    owner.lock()->Destroy(afterSeconds);
}

SP<SomeWorldComponent::NodeTransform> SomeWorldComponent::Transform() const
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

SP<World> SomeWorldComponent::World() const {
    auto owner = this->owner.lock();
    if (nullptr == owner) { return nullptr; }
    auto ownerNode = SCAST<WorldNode>(owner);

    auto _world = ownerNode->World();
    if (_world.expired()) { return nullptr; }
    auto world = _world.lock();

    return world;
}
