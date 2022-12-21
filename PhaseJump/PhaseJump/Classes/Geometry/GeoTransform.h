#ifndef PJGEOTRANSFORM_H
#define PJGEOTRANSFORM_H

#include "Vector3.h"

namespace PJ
{
    /// <summary>
    /// Geometry transform for a world node
    /// </summary>
    struct GeoTransform
    {
        Vector3 position;

        // TODO: should we be storing quaternion/eulerAngles here?
        Vector3 rotation;
        Vector3 scale;

        GeoTransform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale)
        {
        }

        static GeoTransform const defaultTransform;
    };
}

#endif
