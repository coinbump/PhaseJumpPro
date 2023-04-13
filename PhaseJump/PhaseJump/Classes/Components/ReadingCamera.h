#ifndef PJREADINGCAMERA_H
#define PJREADINGCAMERA_H

#include "SomeCamera.h"
#include "SomeRenderContext.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/9/23
 */
namespace PJ {
    /// Uses reading coordinates (top-left is 0,0). X and y increase right/down
    class ReadingCamera : public SomeCamera {
    public:
        Vector3 WorldToScreen(Vector3 vector, SomeRenderContext& renderContext) override {
            auto owner = this->owner.lock();
            auto cameraPosition = owner->transform->position;
            return vector - cameraPosition;
        }
    };
}

#endif
