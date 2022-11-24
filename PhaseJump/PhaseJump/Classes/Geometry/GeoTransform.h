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
        Vector3 rotation;
        Vector3 scale;

        GeoTransform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale)
        {
        }
    };

    namespace _GeoTransform {
        static GeoTransform const defaultTransform(_Vector3::zero, _Vector3::zero, Vector3(1.0f, 1.0f, 1.0f));
    }
}

#endif
