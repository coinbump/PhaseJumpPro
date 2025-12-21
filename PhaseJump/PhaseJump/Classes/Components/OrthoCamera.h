#pragma once

#include "Camera.h"
#include "SomeRenderContext.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/14/25
 */
namespace PJ {
    class Mesh;

    /// Uses cartesian coordinates (center is 0,0). X increases right. Y
    /// increases up
    class OrthoCamera : public Camera {
    public:
        using Base = Camera;

        /// Specifies the half height of the camera, otherwise the exact screen dimensions are used
        std::optional<float> halfHeight;

        float farClip = 1.0f * Vector3::forward.z;
        float nearClip = 1.0f * Vector3::back.z;

        OrthoCamera() {}

        Vector2 RenderContextSize() const;
        Vector2 RenderContextPixelSize() const;
        Vector2 RenderContextExtents() const;
        Vector2 CameraExtents() const;
        Vector2 CameraSize() const;

        Vector2 WorldToScreenScale() const;

        OrthoCamera& SetHalfHeight(float value) {
            halfHeight = value;
            return *this;
        }

        OrthoCamera& ClearHalfHeight() {
            halfHeight = std::nullopt;
            return *this;
        }

        /// Sets the content scaling for the camera (2 means objects appear twice the size)
        OrthoCamera& SetContentScale(float value) {
            float contextExtent = RenderContextPixelSize().y / 2.0f;

            SetHalfHeight(contextExtent / value);

            return *this;
        }

        OrthoCamera& ClearContentScale() {
            return ClearHalfHeight();
        }

        virtual ~OrthoCamera();

        // MARK: Camera

        Vector2 WorldToScreen(Vector3 position) override;
        Vector3 ScreenToWorld(Vector2 position) override;
        void RenderStart(SomeRenderContext* context) override;
        bool IsCulled(Mesh const& mesh) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "OrthoCamera";
        }
    };
} // namespace PJ
