#ifndef PJSOMEPOSITION_H
#define PJSOMEPOSITION_H

#include "Vector3.h"

/*
 RATING: 4 stars
 Needs conversion logic
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
        virtual Vector3 Position() const = 0;
        virtual void SetPosition(Vector3 position) = 0;

        operator Vector3() const { return Position(); }
    };

    class WorldPosition : public SomePosition
    {
    public:
        Vector3 position;

        Vector3 Position() const override { return position; }
        void SetPosition(Vector3 value) override { position = value; }

        WorldPosition() {
        }

        WorldPosition(Vector3 position) : position(position)
        {
        }

        String ToString() const { return "World: " + position.ToString(); }
    };

    class LocalPosition : public SomePosition
    {
    public:
        Vector3 position;
        std::weak_ptr<WorldNode> reference;

        LocalPosition() {
        }

        LocalPosition(Vector3 position, std::weak_ptr<WorldNode> reference) : position(position), reference(reference)
        {
        }

        Vector3 Position() const override { return position; }
        void SetPosition(Vector3 value) override { position = value; }

        String ToString() const { return "Local: " + position.ToString(); }
    };

    class ScreenPosition : public SomePosition
    {
    public:
        Vector2 position;

        ScreenPosition() {
        }

        ScreenPosition(Vector2 position) : position(position) {
        }

        operator Vector2() const { return position; }

        Vector3 Position() const override { return Vector3(position.x, position.y, 0); }
        void SetPosition(Vector3 value) override { position = Vector2(value.x, value.y); }

        String ToString() const { return "Screen: " + position.ToString(); }
    };
}

#endif
