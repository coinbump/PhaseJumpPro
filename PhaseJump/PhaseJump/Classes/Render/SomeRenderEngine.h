#ifndef PJSOMERENDERENGINE_H
#define PJSOMERENDERENGINE_H

#include "Base.h"

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    class RenderModel;

    /// A render engine should be able to perform standard render operations
    class SomeRenderEngine : public Base {
    public:
        virtual void Render(RenderModel const& model) = 0;

        virtual void SetLineWidth(float lineWidth) = 0;
        virtual void EnableFeature(String featureId, bool isEnabled) = 0;

        virtual void RenderLineLoop(VectorList<float> const& vertices) = 0;
        virtual void RenderLine(VectorList<float> const& vertices) = 0;
    };
}

#endif
