#ifndef PJNODE2D_H
#define PJNODE2D_H

#include "RigidBody2D.h"
#include "SomeNode.h"

namespace PJ
{
    /// <summary>
    /// Base 2D world node
    /// </summary>
    class Node2D : public SomeNode
    {
    public:
        using Base = SomeNode;

        bool IsKinematic() const override
        {
            return nullptr == rigidbody ? true : rigidbody->IsKinematic();
        }

    protected:
        std::shared_ptr<RigidBody2D> rigidbody;

        void Awake() override
        {
            Base::Awake();

            rigidbody = GetComponent<RigidBody2D>();
        }

    public:
        /// <summary>
        /// Move this node to the position in 2D space
        /// (NOTE: This uses Vector3, but Rigidbody2D ignores the z component)
        /// </summary>
        void MoveToPosition(Vector3 position, bool force = false) override
        {
            auto transform = Transform();
            if (!transform) { return; }

            if (IsKinematic() || force)
            {
                transform->position = position;
            }
            else if (rigidbody)
            {
                rigidbody->MovePosition(position);
            }
        }
    };
}

#endif
