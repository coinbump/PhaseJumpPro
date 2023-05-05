#include "Vector3.h"
#include <TSVector3D.h>

using namespace PJ;

Vector3 const Vector3::one(1, 1, 1);
Vector3 const Vector3::zero(0, 0, 0);

Vector3 const Vector3::forward(0, 0, -1);
Vector3 const Vector3::back(0, 0, 1);

Vector3::operator Terathon::Vector3D() const { return Terathon::Vector3D(x, y, z); }
Vector3::operator Terathon::Point3D() const { return Terathon::Point3D(x, y, z); }
