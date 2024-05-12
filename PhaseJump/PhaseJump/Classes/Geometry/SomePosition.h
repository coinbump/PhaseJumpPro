#ifndef PJSOMEPOSITION_H
#define PJSOMEPOSITION_H

#include "Vector3.h"
#include "Macros.h"

/*
 RATING: 5 stars
 Needs conversion logic. Has unit tests
 CODE REVIEW: 12/25/22
 */
namespace PJ
{
    class WorldNode;
    
    /// <summary>
    /// Encapsulates both position and coordinates used (world/screen/local)
    /// </summary>
    class SomePosition
    {
    public:
        virtual ~SomePosition() {}
        
        virtual Vector3 Position() const = 0;
        virtual void SetPosition(Vector3 position) = 0;

        operator Vector3() const { return Position(); }
    };

    class WorldPosition : public Vector3, public SomePosition
    {
    public:
        using Base = Vector3;

        Vector3 Position() const override { return *this; }
        void SetPosition(Vector3 value) override { *this = value; }

        WorldPosition() {
        }

        WorldPosition(Vector3 position) : Base(position)
        {
        }

        String ToString() const { return "World: " + Base::ToString(); }
    };

    class LocalPosition : public Vector3, public SomePosition
    {
    public:
        using Base = Vector3;

        WP<WorldNode> reference;

        LocalPosition() {
        }

        LocalPosition(Vector3 position, WP<WorldNode> reference) : Base(position), reference(reference)
        {
        }

        Vector3 Position() const override { return *this; }
        void SetPosition(Vector3 value) override { *((Base *)this) = value; }

        String ToString() const { return "Local: " + Base::ToString(); }
    };

    class ScreenPosition : public Vector2, public SomePosition
    {
    public:
        using Base = Vector2;

        ScreenPosition() {
        }

        ScreenPosition(Vector2 position) : Vector2(position) {
        }

        Vector3 Position() const override { return Vector3(this->x, this->y, 0); }
        void SetPosition(Vector3 value) override { *this = Vector2(value.x, value.y); }

        String ToString() const { return "Screen: " + Base::ToString(); }
    };
}

#endif
