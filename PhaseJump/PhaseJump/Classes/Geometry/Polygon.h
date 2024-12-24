#pragma once

#include "Utils.h"
#include "Vector3.h"
#include "VectorList.h"
#include <algorithm>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    enum class PolyClose { Open, Closed };

    /// Stores vertices for a closed polygon
    /// Polygons are assumed to be closed between the final and first vertices
    class Polygon {
    public:
        using This = Polygon;

    protected:
        VectorList<Vector2> value;

        /// Cached min value
        mutable Vector2 min{};

        /// Cached max value
        mutable Vector2 max{};

        /// If true, the bounds needs to be recalculated
        mutable bool isBoundsInvalid = true;

        Vector2 CalculateMin() const;
        Vector2 CalculateMax() const;

    public:
        Polygon() {}

        Polygon(std::initializer_list<Vector2> vertices) :
            value(vertices) {}

        VectorList<Vector2> const& Value() const {
            return value;
        }

        /// @return Returns a square polygon (4 vertices) based on the given size
        static Polygon MakeSquare(float size) {
            return MakeRect(Vector2::Uniform(size));
        }

        /// @return Returns a rect polygon (4 vertices) based on the given size
        static Polygon MakeRect(Vector2 size) {
            GUARDR_LOG(size.x > 0 && size.y > 0, {}, "ERROR: Can't create empty size polygon");

            float halfWidth = size.x / 2.0f;
            float halfHeight = size.y / 2.0f;
            Polygon result{ { halfWidth * vecLeft, halfHeight * vecUp },
                            { halfWidth * vecRight, halfHeight * vecUp },
                            { halfWidth * vecRight, halfHeight * vecDown },
                            { halfWidth * vecLeft, halfHeight * vecDown } };
            return result;
        }

        void CalculateBoundsIfNeeded() const {
            GUARD(isBoundsInvalid)
            isBoundsInvalid = false;

            min = CalculateMin();
            max = CalculateMax();
        }

        Vector2 Min() const {
            CalculateBoundsIfNeeded();
            return min;
        }

        Vector2 Max() const {
            CalculateBoundsIfNeeded();
            return max;
        }

        Vector2 Size() const {
            auto min = Min();
            auto max = Max();

            return { std::abs(max.x - min.x), std::abs(max.y - min.y) };
        }

        This& SetSize(Vector2 value);

        float Width() const {
            return Size().x;
        }

        float Height() const {
            return Size().y;
        }

        Vector2 Center() const {
            auto min = Min();
            auto size = Size();

            return { min.x + size.x / 2.0f, min.y + size.y / 2.0f };
        }

        bool TestHit(Vector2 pt) const;

        bool IsBoundsInvalid() const {
            return isBoundsInvalid;
        }

        void SetBoundsInvalid() {
            isBoundsInvalid = true;
        }

        This& Add(Vector2 _value) {
            value.push_back(_value);
            isBoundsInvalid = true;
            return *this;
        }

        This& Add(VectorList<Vector2> _value) {
            PJ::AddRange(value, _value);
            isBoundsInvalid = true;
            return *this;
        }

        This& Offset(Vector2 value);

        /// Returns the vertex at the specified index, which is modulo-wrapped if needed
        Vector2 ModGet(size_t index) const {
            auto size = value.size();
            GUARDR(size > 0, {});
            return value[index >= 0 && index < size ? index : index % size];
        }

        size_t Count() const {
            return value.size();
        }

        String ToString() const;

        Vector2& operator[](size_t index) {
            Assert(index >= 0 && index < value.size());
            return value[index];
        }

        Vector2 const& operator[](size_t index) const {
            Assert(index >= 0 && index < value.size());
            return value[index];
        }
    };
} // namespace PJ
