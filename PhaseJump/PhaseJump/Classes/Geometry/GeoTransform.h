#pragma once

#include "Angle.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/3/24
 */
namespace PJ {
    /// Geometry transform for an object in world space
    struct GeoTransform {
        Vector3 position;

        // FUTURE: evaluate storing quaternion/eulerAngles for 3D support
        Vector3 rotation;
        Vector3 scale;
        Vector3 offset;

        struct Config {
            Vector3 position;
            Vector3 rotation;
            Vector3 scale{ 1, 1, 1 };
        };

        GeoTransform(Config const& config) :
            position(config.position),
            rotation(config.rotation),
            scale(config.scale) {}

        Vector3 Position() const {
            return position;
        }

        void SetPosition(Vector3 value) {
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

        Vector3 Scale() const {
            return scale;
        }

        void SetScale(Vector3 value) {
            scale = value;
        }

        Vector3 Rotation() const {
            return rotation;
        }

        Angle Rotation2D() const {
            return Angle::WithDegrees(-rotation.z);
        }

        void SetRotation(Vector3 value) {
            rotation = value;
        }

        void SetRotation(Angle value) {
            // FUTURE: support alternate z-rotation direction if needed
            rotation = Vector3(0, 0, -value.Degrees());
        }

        // FUTURE: Vector3 LocalEulerAngles() const
        // FUTURE: void SetLocalEulerAngles(Vector3 value)
    };
} // namespace PJ
