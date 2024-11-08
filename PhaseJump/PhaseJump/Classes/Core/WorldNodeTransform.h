#pragma once

#include "GeoTransform.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/27/23
 */
namespace PJ {
    class WorldNode;

    /// Keeps track of the object's world position as it is modified
    class WorldNodeTransform {
    public:
        using SetLocalPosFunc = std::function<void(WorldNodeTransform&, Vector3 localPos)>;

    protected:
        GeoTransform value;

    public:
        WorldNode& owner;

        WorldNodeTransform(WorldNode& owner) :
            owner(owner) {}

        WorldNodeTransform(WorldNode& owner, GeoTransform transform) :
            owner(owner),
            value(transform) {}

        GeoTransform& Value() {
            return value;
        }

        GeoTransform const& Value() const {
            return value;
        }

        Vector3 LocalPosition() const {
            return value.position;
        }

        void SetLocalPosition(Vector3 position) {
            value.SetPosition(position);
        }

        float PositionAxisValue(Axis axis) const {
            return value.PositionAxisValue(axis);
        }

        void SetAxisPosition(Axis axis, Vector3 value) {
            this->value.SetAxisPosition(axis, value.AxisValue(axis));
        }

        void SetAxisPosition(Axis axis, float value) {
            this->value.SetAxisPosition(axis, value);
        }

        void SetAxisScale(Axis axis, Vector3 value) {
            this->value.SetAxisScale(axis, value.AxisValue(axis));
        }

        void SetAxisScale(Axis axis, float value) {
            this->value.SetAxisScale(axis, value);
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

        // TODO: what is this func doing and why do we need it?
        void SetWorldPosition(
            Vector3 position,
            SetLocalPosFunc func = [](WorldNodeTransform& transform,
                                      Vector3 localPos) { transform.SetLocalPosition(localPos); }
        );

        /// Set only x,y world positon. Leave z intact
        void SetWorldPositionXY(Vector3 position);

        Angle Rotation2D() const {
            return value.Rotation2D();
        }

        void SetRotation2D(Angle angle) {
            value.SetRotation2D(angle);
        }
    };
} // namespace PJ
