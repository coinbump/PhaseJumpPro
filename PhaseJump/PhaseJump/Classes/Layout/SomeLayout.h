#ifndef PJSOMELAYOUT_H
#define PJSOMELAYOUT_H

#include "Vector3.h"
#include "WorldComponent.h"
#include <memory>

/*
 RATING: 5 stars
 Simple layout
 CODE REVIEW: 12/27/22
 */
namespace PJ
{
    /// <summary>
    /// Defines the layout of child game objects
    /// (different than Unity's Layout Group for UI, which requires a Canvas)
    /// </summary>
    class SomeLayout : public WorldComponent
    {
    public:
        using Base = WorldComponent;

        /// <summary>
        /// If true, layout will be updated automatically.
        /// If false, you must update layout manually
        /// </summary>
        bool autoApply = true;

        /// <summary>
        /// Return the bounds-size of the layout
        /// </summary>
        /// <returns></returns>
        virtual Vector3 Size() const
        {
            return Vector3::zero;
        }

        /// <summary>
        /// Arrange child objects according to the layout
        /// </summary>
        virtual void ApplyLayout() = 0;

    protected:
        void Start() override
        {
            Base::Start();

            if (autoApply)
            {
                ApplyLayout();
            }
        }

        void LateUpdate() override
        {
            Base::LateUpdate();

            if (autoApply)
            {
                ApplyLayout();
            }
        }

    public:
        virtual Vector3 LayoutPositionAt(int index)
        {
            return Vector3::zero;
        }
    };
}

#endif
