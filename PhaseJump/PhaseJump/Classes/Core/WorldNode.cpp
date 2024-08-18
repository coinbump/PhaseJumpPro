#include "WorldNode.h"
#include "Matrix4x4.h"
#include "World.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

World* WorldNode::World() const {
    if (world) {
        return world;
    }
    auto parent = SCAST<WorldNode>(Parent());
    if (parent) {
        return parent->World();
    }
    return nullptr;
}

void WorldNode::CheckedAwake() {
    life.CheckedAwake([this]() {
        auto iterComponents = components;
        for (auto& component : iterComponents) {
            component->CheckedAwake();
        }
    });
}

void WorldNode::CheckedStart() {
    life.CheckedStart([this]() {
        auto iterComponents = components;
        for (auto& component : iterComponents) {
            component->CheckedStart();
        }
    });
}

void WorldNode::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    if (destroyCountdown > 0) {
        destroyCountdown -= time.delta;
        if (destroyCountdown <= 0) {
            isDestroyed = true;
        }
    }

    for (auto const& component : components) {
        if (!component->IsEnabled()) {
            continue;
        }
        component->OnUpdate(time);
    }
}

// MARK: - WorldNode::Transform

// TODO: need unit tests
Vector3 WorldNode::NodeTransform::WorldPosition() const {
    auto result = value.position;

    auto owner = &this->owner;
    if (owner) {
        auto world = owner->World();
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
    auto owner = &this->owner;
    if (owner) {
        auto world = owner->World();
        if (world) {
            auto parent = owner->Parent();
            if (parent) {
                auto parentNode = SCAST<WorldNode>(parent);
                auto worldModelMatrix = world->WorldModelMatrix(*parentNode);
                Terathon::Point3D point(position.x, position.y, position.z);
                auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

                value.position = Vector3(localPosition.x, localPosition.y, localPosition.z);
            }
            return;
        }
    }

    value.position = position;
}

void WorldNode::NodeTransform::SetWorldPositionXY(Vector3 position) {
    auto owner = &this->owner;
    if (owner) {
        auto world = owner->World();
        if (world) {
            auto parent = owner->Parent();
            if (parent) {
                auto parentNode = SCAST<WorldNode>(parent);
                auto worldModelMatrix = world->WorldModelMatrix(*parentNode);
                Terathon::Point3D point(position.x, position.y, position.z);
                auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

                value.position =
                    Vector3(localPosition.x, localPosition.y, owner->transform->LocalPosition().z);
            }
            return;
        }
    }

    value.position = position;
}
