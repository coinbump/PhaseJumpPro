#ifndef PJSOMEPOSITION_H
#define PJSOMEPOSITION_H

#include "Vector3.h"
#include "WorldNode.h"

/*
 RATING: 4 stars
 Needs conversion logic
 CODE REVIEW: 12/25/22
 */
namespace PJ
{
    /// <summary>
    /// Encapsulates both position and coordinates used (world/screen/local)
    /// </summary>
    class SomePosition
    {
        virtual Vector3 Position() const = 0;
        virtual void SetPosition(Vector3 position) = 0;
    };

    struct WorldPosition : public SomePosition
    {
        Vector3 position;

        Vector3 Position() const { return position; }
        void SetPosition(Vector3 value) { position = value; }

        WorldPosition(Vector3 position) : position(position)
        {
        }
    };

    struct LocalPosition : public SomePosition
    {
        Vector3 position;
        std::weak_ptr<WorldNode> reference;

        LocalPosition(Vector3 position, std::shared_ptr<WorldNode> reference) : position(position), reference(reference)
        {
        }

        Vector3 Position() const { return position; }
        void SetPosition(Vector3 value) { position = value; }
    };

    struct ScreenPosition : public SomePosition
    {
        Vector2 position;

        ScreenPosition(Vector2 position) : position(position) {
        }

        Vector3 Position() const { return Vector3(position.x, position.y, 0); }
        void SetPosition(Vector3 value) { position = Vector2(value.x, value.y); }
    };
}

#endif
