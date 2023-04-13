#ifndef PJCARTESIANCAMERA_H
#define PJCARTESIANCAMERA_H

#include "SomeCamera.h"
#include "SomeRenderContext.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/9/23
 */
namespace PJ {
    /// Uses cartesian coordinates (center is 0,0). X increases right. Y increases up
    class CartesianCamera : public SomeCamera {
    public:
        Vector3 WorldToScreen(Vector3 vector, SomeRenderContext& renderContext) override {
            auto size = renderContext.Size();
            auto owner = this->owner.lock();
            auto cameraPosition = owner->transform->position;

            Vector3 screenPosition(vector.x() - cameraPosition.x() + size.x() / 2.0f,
                                   cameraPosition.y() + size.y() / 2.0f - vector.y(),
                                   vector.z() - cameraPosition.z());

            return screenPosition;
        }
    };
}

#endif
