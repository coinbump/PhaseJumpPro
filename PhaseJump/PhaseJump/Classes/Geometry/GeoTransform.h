#ifndef PJGEOTRANSFORM_H
#define PJGEOTRANSFORM_H

#include "Angle.h"
#include "Vector3.h"

// CODE REVIEW: ?/23
namespace PJ {
    /// Geometry transform for a world node
    /// Currently this is missing support for hierarchical coordinates
    struct GeoTransform {
        Vector3 position;

        // FUTURE: evaluate storing quaternion/eulerAngles here if we add 3D
        // support
        Vector3 rotation;
        Vector3 scale;
        Vector3 offset;

        GeoTransform(
            Vector3 position = Vector3::zero, Vector3 rotation = Vector3::zero,
            Vector3 scale = Vector3::one
        ) :
            position(position),
            rotation(rotation),
            scale(scale) {}

        // FUTURE: support hierarchical positions
        Vector3 LocalPosition() const {
            return position;
        }

        Vector3 Position() const {
            return position;
        }

        Angle Rotation2D() const {
            return Angle::DegreesAngle(-rotation.z);
        }

        void SetRotation2D(Angle angle) {
            rotation.z = -angle.Degrees();
        }

        void SetLocalPosition(Vector3 value) {
            position = value;
        }

        float PositionAxisValue(Axis axis) const {
            return position.AxisValue(axis);
        }

        void SetAxisPosition(Axis axis, Vector3 value) {
            SetAxisPosition(axis, value.AxisValue(axis));
        }

        void SetAxisPosition(Axis axis, float value) {
            position.AxisValue(axis) = value;
        }

        void SetAxisScale(Axis axis, Vector3 value) {
            SetAxisScale(axis, value.AxisValue(axis));
        }

        void SetAxisScale(Axis axis, float value) {
            scale.AxisValue(axis) = value;
        }

        void SetPosition(Vector3 value) {
            position = value;
        }

        Vector3 Scale() const {
            return scale;
        }

        void SetScale(Vector3 value) {
            this->scale = value;
        }

        void SetRotation(Angle rotation) {
            // TODO: support both z-ordering directions (Geometry::zRotationFactor)
            this->rotation = Vector3(0, 0, -rotation.Degrees());
        }

        // TODO: is this correct?
        Vector3 LocalEulerAngles() const {
            return rotation;
        }

        // TODO: is this correct?
        void SetLocalEulerAngles(Vector3 value) {
            rotation = value;
        }

        static GeoTransform const defaultTransform;
    };
} // namespace PJ

#endif
