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
        SP<RigidBody2D> rigidbody;

        void Awake() override
        {
            Base::Awake();

            rigidbody = TypeComponent<RigidBody2D>();
        }

    public:
        /// <summary>
        /// Move this node to the position in 2D space
        /// (NOTE: This uses Vector3, but Rigidbody2D ignores the z component)
        /// </summary>
        // TODO: rethink naming. Should it be MoveToWorldPosition?
        void MoveToPosition(Vector3 position, bool force = false) override
        {
            auto transform = Transform();
            if (!transform) { return; }

            if (IsKinematic() || force)
            {
                transform->SetWorldPosition(position);
            }
            else if (rigidbody)
            {
                rigidbody->MovePosition(position);
            }
        }

        // Normalized rotation value (0-1.0)
        float RotationNormal() const { return RotationAngle().Degrees() / 360.0f; }
        void SetRotationNormal(float value) {
            SetRotationAngle(Angle::DegreesAngle(360.0f * value));
        }

        Angle RotationAngle(bool clipRotation = false) const
        {
            return clipRotation ? Angle::DegreesAngle(-Transform()->LocalEulerAngles().z).Clipped() : Angle::DegreesAngle(-Transform()->LocalEulerAngles().z);
        }

        Angle ClippedRotationAngle() const { return RotationAngle(true); }

        void SetRotationAngle(Angle value, bool clipRotation = false) {
            auto newAngle = value;
            if (clipRotation)
            {
                newAngle.Clip();
            }

            if (RotationAngle(clipRotation).Degrees() == newAngle.Degrees()) { return; }

            Transform()->SetLocalEulerAngles(Vector3(0, 0, -newAngle.Degrees()));
        }

        void SetClippedRotationAngle(Angle value) { SetRotationAngle(value, true); }
    };
}

#endif
