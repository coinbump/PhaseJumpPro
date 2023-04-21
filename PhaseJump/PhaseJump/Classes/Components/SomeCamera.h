#ifndef PJSOMECAMERA_H
#define PJSOMECAMERA_H

#include "Vector3.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/9/23
 */
namespace PJ
{
    class SomeRenderContext;
    class Matrix4x4;
    
    /// Transforms the positions of an object from object space to screen space
    class SomeCoordinateConverter
    {
    public:
        virtual Vector3 WorldToScreen(Vector3 vector, SomeRenderContext& renderContext) = 0;
    };

    /// A camera component performs coordinate conversion from world to screen space
    class SomeCamera : public WorldComponent, public SomeCoordinateConverter
    {
    public:
        /// # of pixels per world unit
        Vector3 pixelsPerUnit = Vector3::one;

        // Scales all nodes
        float scale = 1.0f;

        virtual void Render(VectorList<std::shared_ptr<WorldNode>> nodes, std::shared_ptr<SomeRenderContext> renderContext);
        virtual Matrix4x4 ModelMatrixFor(WorldNode const& node);
    };
}

#endif
