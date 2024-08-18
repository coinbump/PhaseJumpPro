#pragma once

#include "GeoTransform.h"
#include "Utils.h"

// CODE REVIEW: ?/23
namespace PJ {
    class WorldNode;

    /// Keeps track of the object's world position as it is modified
    struct WorldNodeTransform {
        WorldNode& owner;
        GeoTransform value;

        // We can't set the owner here because it requires shared_from_this
        WorldNodeTransform(WorldNode& owner) :
            owner(owner) {}

        WorldNodeTransform(WorldNode& owner, GeoTransform transform) :
            owner(owner),
            value(transform) {}

        operator GeoTransform() const {
            return value;
        }

        Vector3 LocalPosition() const {
            return value.position;
        }

        void SetLocalPosition(Vector3 position) {
            value.SetPosition(position);
        }

        // MARK: Scale

        Vector3 Scale() const {
            return value.scale;
        }

        void SetScale(Vector3 value) {
            this->value.scale = value;
        }

        void SetScale(float value) {
            SetScale(Vector3::Uniform(value));
        }

        void SetScale2D(Vector2 value) {
            SetScale(Vector3(value.x, value.y, 1.0f));
        }

        void SetScale2D(float value) {
            SetScale(Vector3(value, value, 1.0f));
        }

        Vector3 LocalScale() const {
            return Scale();
        }

        void SetLocalScale(Vector3 value) {
            SetScale(value);
        }

        // MARK: Rotation

        Vector3 Rotation() const {
            return value.rotation;
        }

        void SetRotation(Vector3 value) {
            this->value.rotation = value;
        }

        void SetRotation(Angle rotation) {
            value.SetRotation(rotation);
        }

        Vector3 LocalEulerAngles() const {
            return value.LocalEulerAngles();
        }

        void SetLocalEulerAngles(Vector3 value) {
            this->value.SetLocalEulerAngles(value);
        }

        Vector3 WorldPosition() const;
        void SetWorldPosition(Vector3 position);

        /// Set only x,y world positon. Leave z intact
        void SetWorldPositionXY(Vector3 position);
    };
} // namespace PJ
