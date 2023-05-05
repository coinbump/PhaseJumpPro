#ifndef PJORTHOCAMERA_H
#define PJORTHOCAMERA_H

#include "SomeCamera.h"
#include "SomeRenderContext.h"

namespace PJ {
    /// Uses cartesian coordinates (center is 0,0). X increases right. Y increases up
    class OrthoCamera : public SomeCamera {
    public:
        int halfHeight = 0;

        Vector2 WorldToScreen(Vector3 position) override;
        Vector3 ScreenToWorld(Vector2 position) override;
    };
}

#endif
