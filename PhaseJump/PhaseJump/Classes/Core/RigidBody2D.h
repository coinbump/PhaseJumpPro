#pragma once

#include "WorldComponent.h"

// Unused. Future feature if we add Box2D support
// namespace PJ {
//    class RigidBody2D : public WorldComponent {
//    public:
//        // FUTURE: Add Box2D for Physics (not supported yet
//        bool IsKinematic() const {
//            return true;
//        };
//
//        void MovePosition(Vector2 position) {
//            GUARD(owner)
//            NodeTransform& transform = owner->transform;
//
//            // is this correct? Local or world?
//            transform.SetWorldPosition(Vector3(position.x, position.y,
//            transform.WorldPosition().z)
//            );
//        }
//    };
//} // namespace PJ
