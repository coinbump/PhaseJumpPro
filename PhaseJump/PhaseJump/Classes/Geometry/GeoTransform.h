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

        void SetLocalPosition(Vector3 position) {
            this->position = position;
        }

        void SetPosition(Vector3 position) {
            this->position = position;
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
