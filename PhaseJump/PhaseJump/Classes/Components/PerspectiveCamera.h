#pragma once

#include "Angle.h"
#include "Camera.h"
#include "SomeRenderContext.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/19/26
 */
namespace PJ {
    class Mesh;

    /// Projects the scene using perspective projection (3D).
    /// Camera looks down -Z in view space (OpenGL convention)
    class PerspectiveCamera final : public Camera {
    public:
        using Base = Camera;

        /// Vertical field of view
        Angle fov = Angle::WithDegrees(60);

        float nearClip = 1.0f;
        float farClip = 10000.0f;

        PerspectiveCamera() {}

        Vector2 RenderContextPixelSize() const;

        /// @return Aspect ratio (width / height) of the render context
        float Aspect() const;

        PerspectiveCamera& SetFOV(Angle value) {
            fov = value;
            return *this;
        }

        PerspectiveCamera& SetNearClip(float value) {
            nearClip = value;
            return *this;
        }

        PerspectiveCamera& SetFarClip(float value) {
            farClip = value;
            return *this;
        }

        virtual ~PerspectiveCamera();

        // MARK: Camera

        Vector2 ContextToScreen(Vector3 position) override;
        Vector3 ScreenToContext(Vector2 position) override;
        void RenderStart(SomeRenderContext* context) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "PerspectiveCamera";
        }
    };
} // namespace PJ
