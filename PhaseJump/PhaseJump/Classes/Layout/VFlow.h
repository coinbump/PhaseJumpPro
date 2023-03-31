#ifndef PJVFLOW_H
#define PJVFLOW_H

#include "SomeLayout2D.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/27/22
 */
namespace PJ
{
    /// <summary>
    /// Flow the objects with non-contextual spacing (object size doesn't matter)
    /// </summary>
    class VFlow : public SomeLayout2D
    {
    public:
        float spacing = 1.0f;

        VFlow(float spacing = 1.0f) : spacing(spacing)
        {
        }

        Vector3 Size() const override
        {
            return Vector3(0, spacing * (ChildNodes()->Count() - 1), 0);
        }

        void ApplyLayout() override
        {
            auto firstPos = (Size().y() / 2) * Vector2::up.y();
            auto position = firstPos;

            auto childNodes = ChildNodes();
            for (auto child : (*childNodes))
            {
                child->transform->SetLocalPosition(Vector3(0, position, child->transform->LocalPosition().z()));
                position += spacing * Vector2::down.y();
            }
        }
    };
}

#endif
