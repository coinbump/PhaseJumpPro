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

    /// Transforms the positions of an object from object space to screen space
    class SomeCoordinateConverter {
    public:
        virtual ~SomeCoordinateConverter() {}

        virtual Vector2 WorldToScreen(Vector3 position) = 0;
        virtual Vector3 ScreenToWorld(Vector2 position) = 0;
    };

    /// A camera component performs coordinate conversion from world to screen
    /// space
    class SomeCamera : public WorldComponent<>, public SomeCoordinateConverter {
    public:
        /// Allows each camera to have its own render pipeline
        /// Add render processors to the render system when you want them to be shared between
        /// cameras
        RenderProcessingModel processingModel;

        /// If specified, the camera will render into this context instead of the default
        /// Use when rendering into a texture buffer
        // TODO: WIP
        SP<SomeRenderContext> renderContext;

        virtual void PreRender(RenderContextModel const& contextModel) {}

        virtual bool IsCulled(Mesh const& mesh) = 0;
    };
} // namespace PJ
