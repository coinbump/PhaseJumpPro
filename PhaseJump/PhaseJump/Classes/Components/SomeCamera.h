#pragma once

#include "Vector3.h"
#include "WorldComponent.h"

// CODE REVIEW: 4/9/23
namespace PJ {
    class SomeRenderContext;
    class Matrix4x4;
    class RenderModel;
    class RenderContextModel;
    class RenderProcessModel;

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
        virtual Matrix4x4 Matrix() = 0;

        virtual void PreRender(RenderContextModel const& contextModel) {}
    };
} // namespace PJ
