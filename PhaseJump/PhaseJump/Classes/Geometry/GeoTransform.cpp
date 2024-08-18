#include "GeoTransform.h"

using namespace PJ;

GeoTransform const
    GeoTransform::defaultTransform(Vector3::zero, Vector3::zero, Vector3(1.0f, 1.0f, 1.0f));
