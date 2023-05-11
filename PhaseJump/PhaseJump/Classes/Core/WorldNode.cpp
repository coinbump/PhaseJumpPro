#include "WorldNode.h"
#include "World.h"
#include "Matrix4x4.h"
#include <TSMatrix4D.h>

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
    if (IsAwake()) { return; }

    transform->owner = SCAST<WorldNode>(shared_from_this());

    auto iterComponents = components;
    for (auto component : iterComponents) {
        component->Awake();
    }

    awakeCount++;
}

/// Called for every object after Awake
void WorldNode::Start() {
    if (IsStarted()) { return; }

    auto iterComponents = components;
    for (auto component : iterComponents) {
        component->Start();
    }

    awakeCount++;
}

// MARK: - WorldNode::Transform

// TODO: need unit tests
Vector3 WorldNode::NodeTransform::WorldPosition() const {
    auto result = transform.position;

    auto owner = this->owner.lock();
    if (owner) {
        auto world = owner->World().lock();
        if (world) {
            auto parent = owner->Parent();
            if (parent) {
                auto parentNode = SCAST<WorldNode>(parent);
                auto worldModelMatrix = world->WorldModelMatrix(*parentNode);
                result = worldModelMatrix.MultiplyPoint(result);
                return result;
            }
        }
    }

    return result;
}

// TODO: need unit tests
void WorldNode::NodeTransform::SetWorldPosition(Vector3 position) {
    auto owner = this->owner.lock();
    if (owner) {
        auto world = owner->World().lock();
        if (world) {
            auto parent = owner->Parent();
            if (parent) {
                auto parentNode = SCAST<WorldNode>(parent);
                auto worldModelMatrix = world->WorldModelMatrix(*parentNode);
                Terathon::Point3D point(position.x, position.y, position.z);
                auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

                transform.position = Vector3(localPosition.x, localPosition.y, localPosition.z);
            }
            return;
        }
    }

    transform.position = position;
}

void WorldNode::NodeTransform::SetWorldPositionXY(Vector3 position) {
    auto owner = this->owner.lock();
    if (owner) {
        auto world = owner->World().lock();
        if (world) {
            auto parent = owner->Parent();
            if (parent) {
                auto parentNode = SCAST<WorldNode>(parent);
                auto worldModelMatrix = world->WorldModelMatrix(*parentNode);
                Terathon::Point3D point(position.x, position.y, position.z);
                auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

                transform.position = Vector3(localPosition.x, localPosition.y, owner->transform->LocalPosition().z);
            }
            return;
        }
    }

    transform.position = position;
}
