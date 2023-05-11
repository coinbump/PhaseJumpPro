#ifndef PJRIGIDBODY2D_H
#define PJRIGIDBODY2D_H

#include "WorldComponent.h"

namespace PJ
{
    class RigidBody2D : public WorldComponent
    {
    public:
        // FUTURE: Add Box2D for Physics (not supported yet
        bool IsKinematic() const { return true; };
        void MovePosition(Vector2 position) {
            auto transform = Transform();
            if (!transform) { return; }

            // TODO: is this correct? Local or world?
            transform->SetWorldPosition(Vector3(position.x, position.y, transform->WorldPosition().z));
        }
    };
}

#endif
