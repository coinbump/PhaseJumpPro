#ifndef PJGEOTRANSFORM_H
#define PJGEOTRANSFORM_H

#include "Vector3.h"
#include "Angle.h"

namespace PJ
{
    /// <summary>
    /// Geometry transform for a world node
    /// Currently this is missing support for hierarchical coordinates
    /// </summary>
    struct GeoTransform
    {
        Vector3 position;

        // FUTURE: evaluate storing quaternion/eulerAngles here if we add 3D support
        Vector3 rotation;
        Vector3 scale;

        GeoTransform(Vector3 position = Vector3::zero, Vector3 rotation = Vector3::zero, Vector3 scale = Vector3::zero) : position(position), rotation(rotation), scale(scale)
        {
        }

        // FUTURE: support hierarchical positions
        Vector3 LocalPosition() const { return position; }
        Vector3 Position() const { return position; }
        void SetLocalPosition(Vector3 position) { this->position = position; }
        void SetPosition(Vector3 position) { this->position = position; }

        void SetRotation(Angle rotation) { this->rotation = Vector3(0, 0, -rotation.Degrees()); }

        static GeoTransform const defaultTransform;
    };
}

#endif
