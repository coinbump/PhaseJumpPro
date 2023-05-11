#ifndef PJSOMECAMERA_H
#define PJSOMECAMERA_H

#include "Vector3.h"
#include "WorldComponent.h"

// CODE REVIEW: 4/9/23
namespace PJ
{
    class SomeRenderContext;
    class Matrix4x4;
    
    /// Transforms the positions of an object from object space to screen space
    class SomeCoordinateConverter
    {
    public:
        virtual Vector2 WorldToScreen(Vector3 position) = 0;
        virtual Vector3 ScreenToWorld(Vector2 position) = 0;
    };

    /// A camera component performs coordinate conversion from world to screen space
    class SomeCamera : public WorldComponent, public SomeCoordinateConverter
    {
    public:
        virtual void Render(VectorList<SP<WorldNode>> nodes, SP<SomeRenderContext> renderContext);
    };
}

#endif
