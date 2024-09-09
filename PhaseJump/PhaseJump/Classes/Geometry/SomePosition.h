#pragma once

#include "Utils.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/23
 */
namespace PJ {
    // FUTURE: needs conversion logic
    class WorldNode;

    /// Encapsulates both position and coordinates used (world/screen/local)

    class WorldPosition : public Vector3 {
    public:
        using Base = Vector3;

        WorldPosition() {}

        WorldPosition(Vector3 position) :
            Base(position) {}

        String ToString() const {
            return "World: " + Base::ToString();
        }
    };

    // TODO: Re-evaluate. Do we really need these?
    class LocalPosition : public Vector3 {
    public:
        using Base = Vector3;

        LocalPosition() {}

        LocalPosition(Vector3 position) :
            Base(position) {}

        String ToString() const {
            return "Local: " + Base::ToString();
        }
    };

    // TODO: Re-evaluate. Do we really need these?
    class ScreenPosition : public Vector2 {
    public:
        using Base = Vector2;

        ScreenPosition() {}

        ScreenPosition(Vector2 position) :
            Vector2(position) {}

        void SetPosition(Vector3 value) {
            *this = Vector2(value.x, value.y);
        }

        String ToString() const {
            return "Screen: " + Base::ToString();
        }
    };

    // Ensure that we haven't added virtual methods to these basic types
    static_assert(!std::is_polymorphic_v<ScreenPosition>);
    static_assert(!std::is_polymorphic_v<WorldPosition>);
    static_assert(!std::is_polymorphic_v<LocalPosition>);
} // namespace PJ
