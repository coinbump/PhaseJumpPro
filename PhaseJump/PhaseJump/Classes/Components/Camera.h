#pragma once

#include "RenderProcessingModel.h"
#include "Vector3.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 9/2/24
 */
namespace PJ {
    class RenderProcessingModel;
    class RenderContextModel;
    class Mesh;
    class SomeRenderContext;

    enum class CameraType {
        /// Renders all nodes in world
        World,

        /// Renders child nodes only
        Viewport
    };

    enum class CameraProjection {
        /// Orthographic projection (2D)
        Orthographic,

        /// Perspective projection (3D)
        Perspective
    };

    /// A camera component performs coordinate conversion from world to screen
    /// space
    class Camera : public WorldComponent {
    public:
        CameraType type = CameraType::World;
        Color clearColor = Color::white;

        /// Allows each camera to have its own render pipeline
        /// Add render processors to the render system when you want them to be shared between
        /// cameras
        RenderProcessingModel processingModel;

        /// If specified, the camera will render into this context instead of the default
        /// Use when rendering into a texture buffer
        SP<SomeRenderContext> renderContext;

        Camera();

        virtual void RenderStart(SomeRenderContext* context) {}

        /// Transforms position from camera coordinates to screen space coordinates
        virtual Vector2 ContextToScreen(Vector3 position) = 0;

        /// Transforms position from screen space to camera coordinates
        virtual Vector3 ScreenToContext(Vector2 position) = 0;

        /// Transforms a position from this camera's local cartesian coordinate system into
        /// outer-world coordinates.
        virtual Vector3 LocalToWorld(Vector3 localPos) {
            return localPos;
        }

        /// Inverse of LocalToWorld. Converts an outer-world position into this camera's local
        /// cartesian coordinate system.
        virtual Vector3 WorldToLocal(Vector3 worldPos) {
            return worldPos;
        }

        /// Converts a screen position into this camera's local cartesian frame — the frame
        /// that descendant nodes' WorldModelMatrix positions live in.
        virtual Vector3 ScreenToLocal(Vector2 screen) {
            return ScreenToContext(screen);
        }

        /// Inverse of ScreenToLocal.
        virtual Vector2 LocalToScreen(Vector3 localPos) {
            return ContextToScreen(localPos);
        }

        virtual bool IsCulled(Mesh const& mesh) {
            return false;
        }
    };
} // namespace PJ
