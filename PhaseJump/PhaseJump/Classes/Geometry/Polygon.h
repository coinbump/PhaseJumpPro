#ifndef PJPOLYGON_H
#define PJPOLYGON_H

#include "Utils.h"
#include "Vector3.h"
#include "VectorList.h"
#include <algorithm>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/23
 */
namespace PJ {
    // TODO: does it make sense for this to use Vector2 instead?
    /// Stores vertices for closed polygon
    struct Polygon {
        VectorList<Vector3> value;

        Polygon() {}

        Polygon(std::initializer_list<Vector3> vertices) :
            value(vertices) {}

        /// @return Returns a square polygon (4 vertices) based on the given size
        static Polygon MakeSquare(float size) {
            return MakeRect(Vector2::Uniform(size));
        }

        /// @return Returns a rect polygon (4 vertices) based on the given size
        static Polygon MakeRect(Vector2 size) {
            float halfWidth = size.x / 2.0f;
            float halfHeight = size.y / 2.0f;
            Polygon result{ { halfWidth * vecLeft, halfHeight * vecUp, 0 },
                            { halfWidth * vecRight, halfHeight * vecUp, 0 },
                            { halfWidth * vecRight, halfHeight * vecDown, 0 },
                            { halfWidth * vecLeft, halfHeight * vecDown, 0 } };
            return result;
        }

        Vector3 Min() const {
            if (value.size() == 0) {
                return Vector3::zero;
            }

            auto result = value[0];

            for (auto& vertex : value) {
                result = Vector3(
                    std::min(result.x, vertex.x), std::min(result.y, vertex.y),
                    std::min(result.z, vertex.z)
                );
            }

            return result;
        }

        Vector3 Max() const {
            if (value.size() == 0) {
                return Vector3::zero;
            }

            auto result = value[0];

            for (auto& vertex : value) {
                result = Vector3(
                    std::max(result.x, vertex.x), std::max(result.y, vertex.y),
                    std::max(result.z, vertex.z)
                );
            }

            return result;
        }

        Vector3 Size() const {
            auto min = Min();
            auto max = Max();

            return Vector3(
                std::abs(max.x - min.x), std::abs(max.y - min.y), std::abs(max.z - min.z)
            );
        }

        float Width() const {
            return Size().x;
        }

        float Height() const {
            return Size().y;
        }

        Vector3 Center() const {
            auto min = Min();
            auto size = Size();

            return Vector3(min.x + size.x / 2.0f, min.y + size.y / 2.0f, min.z + size.z / 2.0f);
        }

        bool TestHit(Vector2 pt) const;

        void Add(Vector3 value) {
            this->value.push_back(value);
        }

        Vector3 At(size_t index) const {
            auto size = value.size();
            GUARDR(size > 0, Vector3::zero);
            return value[index >= 0 && index < size ? index : index % size];
        }

        size_t Count() const {
            return value.size();
        }

        String ToString() const;

        Vector3& operator[](size_t index) {
            Assert(index >= 0 && index < value.size());
            return value[index];
        }

        Vector3 const& operator[](size_t index) const {
            Assert(index >= 0 && index < value.size());
            return value[index];
        }
    };
} // namespace PJ

#endif
