#ifndef PJORTHOCAMERA_H
#define PJORTHOCAMERA_H

#include "SomeCamera.h"
#include "SomeRenderContext.h"

namespace PJ {
    /// Uses cartesian coordinates (center is 0,0). X increases right. Y increases up
    class OrthoCamera : public SomeCamera {
    public:
        int halfHeight = 0;

        // TODO: rethink this
        Vector3 WorldToScreen(Vector3 vector, SomeRenderContext& renderContext) override {
            auto size = renderContext.Size();
            auto owner = this->owner.lock();
            auto cameraPosition = owner->transform->position;

            Vector3 screenPosition(vector.x - cameraPosition.x + size.x / 2.0f,
                                   cameraPosition.y + size.y / 2.0f - vector.y,
                                   vector.z - cameraPosition.z);

            return screenPosition;
        }
    };
}

#endif
