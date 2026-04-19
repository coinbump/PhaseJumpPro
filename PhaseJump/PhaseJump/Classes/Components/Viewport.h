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

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Viewport";
        }

        void Awake() override;

    protected:
        void ResizeBuffer(Vector2 size);
    };
} // namespace PJ
