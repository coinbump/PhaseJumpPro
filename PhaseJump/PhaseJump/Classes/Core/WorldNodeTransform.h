#ifndef PJWORLDNODETRANSFORM_H
#define PJWORLDNODETRANSFORM_H

#include "GeoTransform.h"
#include "Macros.h"

namespace PJ {
    class WorldNode;

    /// Keeps track of the object's world position as it is modified
    struct WorldNodeTransform {
        WP<WorldNode> owner;
        GeoTransform transform;

        // We can't set the owner here because it requires shared_from_this
        WorldNodeTransform() {
        }

        WorldNodeTransform(GeoTransform transform) : transform(transform) {
        }

        operator GeoTransform() const { return transform; }

        Vector3 LocalPosition() const { return transform.position; }
        void SetLocalPosition(Vector3 position) { transform.SetPosition(position); }

        Vector3 Scale() const { return transform.scale; }
        void SetScale(Vector3 const& value) { transform.scale = value; }
        Vector3 LocalScale() const { return transform.scale; }
        void SetLocalScale(Vector3 const& value) { transform.scale = value; }

        Vector3 Rotation() const { return transform.rotation; }
        void SetRotation(Vector3 const& value) { transform.rotation = value; }
        void SetRotation(Angle rotation) { transform.SetRotation(rotation); }

        Vector3 LocalEulerAngles() const { return transform.LocalEulerAngles(); }
        void SetLocalEulerAngles(Vector3 value) { transform.SetLocalEulerAngles(value); }

        Vector3 WorldPosition() const;
        void SetWorldPosition(Vector3 position);

        /// Set only x,y world positon. Leave z intact
        void SetWorldPositionXY(Vector3 position);
    };
}

#endif
