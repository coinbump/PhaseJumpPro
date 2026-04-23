#pragma once

#include "Camera.h"
#include "ObservedValue.h"
#include "Vector2.h"

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 4/17/26
 */
namespace PJ {
    /// Camera that renders into an offscreen texture buffer.
    /// Use `renderContext->GetTexture()` for renders
    class Viewport : public Camera {
    public:
        using Base = Camera;
        using This = Viewport;

        ObservedValue<Vector2> worldSize;

        Viewport();

        /// Replaces this viewport's backing texture and matches the viewport's
        /// worldSize to the texture's pixel size
        void Swap(SP<Texture> texture);

        /// @return Outer-world cartesian position for a point in this viewport's buffer context
        Vector3 ContextToWorld(Vector3 position) const;

        /// @return Buffer context position for an outer-world cartesian position.
        Vector3 WorldToContext(Vector3 worldPos) const;

        // MARK: Camera

        void RenderStart(SomeRenderContext* context) override;

        /// Converts buffer-local context coords to SDL3 screen coords
        Vector2 ContextToScreen(Vector3 position) override;

        /// Converts SDL3 screen coords to buffer-local context coords
        Vector3 ScreenToContext(Vector2 position) override;

        /// Maps a position in the viewport's local cartesian space (where descendant nodes
        /// report their WorldModelMatrix-based world positions) into outer-world coordinates,
        Vector3 LocalToWorld(Vector3 localPos) override;

        /// Inverse of LocalToWorld.
        Vector3 WorldToLocal(Vector3 worldPos) override;

        /// Routes screen position through the outer main camera and into this viewport's local
        /// frame, so a raw screen click lands where descendant nodes live.
        Vector3 ScreenToLocal(Vector2 screen) override;

        /// Inverse of ScreenToLocal.
        Vector2 LocalToScreen(Vector3 localPos) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Viewport";
        }

        void Awake() override;

    protected:
        void ResizeBuffer(Vector2 size);
    };
} // namespace PJ
