#pragma once

#include "SomeCamera.h"
#include "SomeRenderContext.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/2/24
 */
namespace PJ {
    class Mesh;

    /// Uses cartesian coordinates (center is 0,0). X increases right. Y
    /// increases up
    class OrthoCamera : public SomeCamera {
    public:
        enum class Frustrum {
            /// Z position isn't take into account when culling objects for render
            /// Useful when using a single camera
            None,

            /// Only objects inside the camera frustrum will be rendered
            /// Useful when using multiple cameras
            Enabled
        };

        using Base = SomeCamera;

        /// Specifies the half height of the camera, otherwise the exact screen dimensions are used
        std::optional<float> halfHeight;

        // TODO: this doesn't make sense, OpenGL always uses a frustrum. Rethink this
        Frustrum frustrum = Frustrum::None;

        float farClip = 1.0f * Vector3::forward.z;
        float nearClip = 1.0f * Vector3::back.z;

        OrthoCamera(Frustrum frustrum = Frustrum::Enabled) :
            frustrum(frustrum) {}

        Vector2 RenderContextSize() const;
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
            float contextExtent = RenderContextSize().y / 2.0f;

            SetHalfHeight(contextExtent / value);

            return *this;
        }

        OrthoCamera& ClearContentScale() {
            return ClearHalfHeight();
        }

        virtual ~OrthoCamera();

        // MARK: SomeCamera

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
