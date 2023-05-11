#ifndef PJPOLYGON_H
#define PJPOLYGON_H

#include "Vector3.h"
#include "VectorList.h"
#include <algorithm>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/25/22
 */
namespace PJ
{
    /// <summary>
    /// Stores vertices for closed polygon
    /// </summary>
    struct Polygon : public VectorList<Vector3>
    {
        Vector3 Min() const
        {
            if (this->Count() == 0) { return Vector3::zero; }

            auto result = (*this)[0];

            for (auto vertex : (*this))
            {
                result = Vector3(std::min(result.x, vertex.x),
                                 std::min(result.y, vertex.y),
                                 std::min(result.z, vertex.z));
            }

            return result;
        }

        Vector3 Max() const
        {
            if (this->Count() == 0) { return Vector3::zero; }

            auto result = (*this)[0];

            for (auto vertex : (*this))
            {
                result = Vector3(std::max(result.x, vertex.x),
                                 std::max(result.y, vertex.y),
                                 std::max(result.z, vertex.z));
            }

            return result;
        }

        Vector3 Size() const
        {
            auto min = Min();
            auto max = Max();

            return Vector3(std::abs(max.x - min.x), std::abs(max.y - min.y), std::abs(max.z - min.z));
        }

        Vector3 Center() const
        {
            auto min = Min();
            auto size = Size();

            return Vector3(min.x + size.x / 2.0f, min.y + size.y / 2.0f, min.z + size.z / 2.0f);
        }

        bool TestHit(Vector2 pt) const;

        Vector3 At(size_t index) const { return (*this)[index]; }
        Vector3& At(size_t index) { return (*this)[index]; }

        String ToString() const;
    };
}

#endif
