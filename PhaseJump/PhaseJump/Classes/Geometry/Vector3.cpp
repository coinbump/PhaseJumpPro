#include "Vector3.h"
#include <TSVector3D.h>

using namespace PJ;

Vector3 const Vector3::forward(0, 0, 1);
Vector3 const Vector3::back(0, 0, -1);

Vector3::operator Terathon::Vector3D() const {
    return Terathon::Vector3D(x, y, z);
}

Vector3::operator Terathon::Point3D() const {
    return Terathon::Point3D(x, y, z);
}

std::ostream& PJ::operator<<(std::ostream& os, Vector3 const& value) {
    os << "{" << value.x << ", " << value.y << ", " << value.z << "}";
    return os;
}

String Vector3::ToString() const {
    std::stringstream stream;
    stream << *this;
    return stream.str();
}

// MARK: Vector3Int

std::ostream& PJ::operator<<(std::ostream& os, Vector3Int const& value) {
    os << "{" << value.x << ", " << value.y << ", " << value.z << "}";
    return os;
}

String Vector3Int::ToString() const {
    std::stringstream stream;
    stream << *this;
    return stream.str();
}
