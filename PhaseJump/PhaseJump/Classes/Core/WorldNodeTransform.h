#pragma once

#include "GeoTransform.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/3/24
 */
namespace PJ {
    class WorldNode;

    /// Transform for a world node
    class WorldNodeTransform {
    public:
        /// Used to customize behavior for SetWorldPosition
        using SetLocalPosFunc = std::function<void(WorldNodeTransform&, Vector3 localPos)>;

    protected:
        GeoTransform geo;

    public:
        WorldNode& owner;

        WorldNodeTransform(WorldNode& owner) :
            owner(owner),
            geo({}) {}

        WorldNodeTransform(WorldNode& owner, GeoTransform transform) :
            owner(owner),
            geo(transform) {}

        GeoTransform& Value() {
            return geo;
        }

        GeoTransform const& Value() const {
            return geo;
        }

        Vector3 LocalPosition() const {
            return geo.position;
        }

        void SetLocalPosition(Vector3 position) {
            geo.SetPosition(position);
        }

        float PositionAxisValue(Axis axis) const {
            return geo.PositionAxisValue(axis);
        }

        void SetAxisPosition(Axis axis, Vector3 value) {
            geo.SetAxisPosition(axis, value.AxisValue(axis));
        }

        void SetAxisPosition(Axis axis, float value) {
            geo.SetAxisPosition(axis, value);
        }

        void SetAxisScale(Axis axis, Vector3 value) {
            geo.SetAxisScale(axis, value.AxisValue(axis));
        }

        void SetAxisScale(Axis axis, float value) {
            geo.SetAxisScale(axis, value);
        }

        // MARK: Scale

        Vector3 Scale() const {
            return geo.scale;
        }

        void SetScale(Vector3 value) {
            this->geo.scale = value;
        }

        void SetScale(float value) {
            SetScale(Vector3::Uniform(value));
        }

        void SetScale2D(Vector2 value) {
            SetScale({ value.x, value.y, 1.0f });
        }

        void SetScale2D(float value) {
            SetScale({ value, value, 1.0f });
        }

        // MARK: Rotation

        Vector3 Rotation() const {
            return geo.rotation;
        }

        void SetRotation(Vector3 value) {
            geo.rotation = value;
        }

        Angle Rotation2D() const {
            return geo.Rotation2D();
        }

        void SetRotation(Angle angle) {
            geo.SetRotation(angle);
        }

        Vector3 WorldPosition() const;

        /// Set the world position, with a func that applies the value to local position (we might
        /// not want to transform the z value in 2D space)
        void SetWorldPosition(
            Vector3 position,
            SetLocalPosFunc func = [](WorldNodeTransform& transform,
                                      Vector3 localPos) { transform.SetLocalPosition(localPos); }
        );

        /// Set only x,y world positon. Leave z intact
        void SetWorldPositionXY(Vector3 position);
    };
} // namespace PJ
