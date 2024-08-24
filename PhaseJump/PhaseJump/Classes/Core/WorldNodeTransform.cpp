#include "WorldNodeTransform.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "World.h"
#include "WorldNode.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

// TODO: need unit tests
Vector3 WorldNodeTransform::WorldPosition() const {
    auto result = value.position;

    auto owner = &this->owner;
    if (owner) {
        auto world = owner->World();
        if (world) {
            auto parentNode = owner->Parent();
            if (parentNode) {
                auto worldModelMatrix = world->WorldModelMatrix(*parentNode);
                result = worldModelMatrix.MultiplyPoint(result);
                return result;
            }
        }
    }

    return result;
}

// TODO: need unit tests
void WorldNodeTransform::SetWorldPosition(Vector3 position) {
    auto owner = &this->owner;
    if (owner) {
        auto world = owner->World();
        if (world) {
            auto parentNode = owner->Parent();
            if (parentNode) {
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

void WorldNodeTransform::SetWorldPositionXY(Vector3 position) {
    auto owner = &this->owner;
    if (owner) {
        auto world = owner->World();
        if (world) {
            auto parentNode = owner->Parent();
            if (parentNode) {
                auto worldModelMatrix = world->WorldModelMatrix(*parentNode);
                Terathon::Point3D point(position.x, position.y, position.z);
                auto localPosition = Terathon::InverseTransform(worldModelMatrix, point);

                value.position =
                    Vector3(localPosition.x, localPosition.y, owner->transform.LocalPosition().z);
            }
            return;
        }
    }

    value.position = position;
}
