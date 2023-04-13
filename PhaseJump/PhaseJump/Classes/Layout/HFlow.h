#ifndef PJHFLOW_H
#define PJHFLOW_H

#include "SomeLayout2D.h"

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
    class HFlow : public SomeLayout2D
    {
    public:
        float spacing = 1.0f;

        HFlow(float spacing = 1.0f) : spacing(spacing)
        {
        }

        Vector3 Size() const override
        {
            return Vector3(spacing * (ChildNodes().Count() - 1), 0, 0);
        }

        void ApplyLayout() override
        {
            auto firstPos = -Size().x() / 2;
            auto position = firstPos;

            auto childNodes = ChildNodes();
            for (auto child : childNodes)
            {
                child->transform->SetLocalPosition(Vector3(position, 0, child->transform->LocalPosition().z()));
                position += spacing;
            }
        }
    };
}

#endif
