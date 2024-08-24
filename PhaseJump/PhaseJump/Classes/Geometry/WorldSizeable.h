#pragma once

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/24/24
 */
namespace PJ {
    /// Interface to an object that can return its world size
    class WorldSizeable {
    public:
        virtual ~WorldSizeable() {}

        virtual std::optional<Vector3> WorldSize() const {
            return std::nullopt;
        }

        virtual void SetWorldSize(Vector3 value) = 0;
    };
} // namespace PJ
