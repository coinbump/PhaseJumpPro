#ifndef PJREADINGCAMERA_H
#define PJREADINGCAMERA_H

#include "SomeCamera.h"
#include "SomeRenderContext.h"

// TODO: needs rethink. Is this a camera or something else?
namespace PJ {
    /// Uses reading coordinates (top-left is 0,0). X and y increase right/down
    //    class ReadingCamera : public SomeCamera {
    //    public:
    //        Vector3 WorldToScreen(Vector3 vector, SomeRenderContext&
    //        renderContext) override {
    //            auto owner = this->owner.lock();
    //            auto cameraPosition = owner->transform.position;
    //            return vector - cameraPosition;
    //        }
    //    };
} // namespace PJ

#endif
