#include "Mesh.h"

using namespace std;
using namespace PJ;

using This = Mesh;

Mesh::Mesh() {
    calculateBoundsFunc = [](auto& mesh) {
        bool hasFirst = false;
        Vector3 minValue;
        Vector3 maxValue;

        for (Vector3 const& v : mesh.vertices) {
            if (!hasFirst) {
                hasFirst = true;
                minValue = v;
                maxValue = v;
                continue;
            }

            minValue = Vector3(
                std::min(minValue.x, v.x), std::min(minValue.y, v.y), std::min(minValue.z, v.z)
            );
            maxValue = Vector3(
                std::max(maxValue.x, v.x), std::max(maxValue.y, v.y), std::max(maxValue.z, v.z)
            );
        }

        Vector3 extents = (maxValue - minValue) / 2.0f;
        Vector3 center = minValue + extents;
        return Bounds(center, extents);
    };
}

void Mesh::Update(
    VectorList<Vector3> const& vertices, VectorList<uint32_t> const& triangles,
    VectorList<Vector2> const& uvs
) {
    this->vertices = vertices;
    this->triangles = triangles;
    this->uvs = uvs;

    needsCalculate = true;
}

void Mesh::CalculateIfNeeded() const {
    GUARD(needsCalculate)
    needsCalculate = false;

    CalculateBounds();
    CalculateNormals();
}

Mesh Mesh::operator+(Mesh const& rhs) const {
    Mesh result = *this;
    result += rhs;
    return result;
}

Mesh& Mesh::operator+=(Mesh const& rhs) {
    GUARDR(!IsEmpty(rhs.vertices), *this)

    auto& result = *this;
    uint32_t triangleOffset = (uint32_t)result.vertices.size();

    AddRange(result.vertices, rhs.vertices);
    AddRange(result.uvs, rhs.uvs);

    VectorList<uint32_t> newTriangles;
    newTriangles.assign(rhs.Triangles().begin(), rhs.Triangles().end());
    for (auto& triangle : newTriangles) {
        triangle += triangleOffset;
    }

    AddRange(result.triangles, newTriangles);

    result.needsCalculate = true;

    return result;
}

Mesh& Mesh::operator*=(Matrix4x4 const& rhs) {
    auto& result = *this;
    std::transform(
        result.vertices.begin(), result.vertices.end(), result.vertices.begin(),
        [&](Vector3 v) { return rhs.MultiplyPoint(v); }
    );
    return result;
}

Mesh Mesh::operator*(Matrix4x4 const& rhs) const {
    Mesh result = *this;
    result *= rhs;
    return result;
}

This& Mesh::Offset(Vector3 offset) {
    for (auto& v : vertices) {
        v += offset;
    }

    return *this;
}
