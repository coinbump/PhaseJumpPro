#include "WorldNode.h"
#include "World.h"

using namespace std;
using namespace PJ;

std::weak_ptr<World> WorldNode::World() const {
    if (!world.expired()) {
        return world;
    }
    auto parent = SCAST<WorldNode>(Parent());
    if (parent) {
        return parent->World();
    }
    return std::weak_ptr<PJ::World>();
}

void WorldNode::Awake() {
    auto iterComponents = components;
    for (auto component : iterComponents) {
        component->Awake();
    }
}

/// Called for every object after Awake
void WorldNode::Start() {
    auto iterComponents = components;
    for (auto component : iterComponents) {
        component->Start();
    }
}
