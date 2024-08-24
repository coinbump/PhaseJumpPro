#ifndef PJORTHOCAMERA_H
#define PJORTHOCAMERA_H

#include "SomeCamera.h"
#include "SomeRenderContext.h"

// CODE REVIEW: ?/23
namespace PJ {
    /// Uses cartesian coordinates (center is 0,0). X increases right. Y
    /// increases up
    class OrthoCamera : public SomeCamera {
    public:
        using Base = SomeCamera;

        int halfHeight = 0;

        virtual ~OrthoCamera();

        // MARK: SomeCamera

        Matrix4x4 Matrix() override;
        Vector2 WorldToScreen(Vector3 position) override;
        Vector3 ScreenToWorld(Vector2 position) override;
        void PreRender(RenderContextModel const& contextModel) override;
    };
} // namespace PJ

#endif
