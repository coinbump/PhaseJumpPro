#ifndef PJRIGIDBODY2D_H
#define PJRIGIDBODY2D_H

#include "WorldComponent.h"

// CODE REVIEW: ?/23
namespace PJ {
    class RigidBody2D : public WorldComponent<> {
    public:
        // FUTURE: Add Box2D for Physics (not supported yet
        bool IsKinematic() const {
            return true;
        };

        void MovePosition(Vector2 position) {
            GUARD(owner)
            NodeTransform& transform = *owner->transform;

            // TODO: is this correct? Local or world?
            transform.SetWorldPosition(Vector3(position.x, position.y, transform.WorldPosition().z)
            );
        }
    };
} // namespace PJ

#endif
