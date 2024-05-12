#ifndef PJSOMERENDERENGINE_H
#define PJSOMERENDERENGINE_H

#include "Base.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    struct RenderModel;

    /// A render engine should be able to perform standard render operations
    class SomeRenderEngine : public Base {
    public:
        virtual void RenderStart() = 0;
        virtual void RenderProcess(RenderModel const& model) = 0;
        virtual void RenderDraw() = 0;

        virtual void SetLineWidth(float lineWidth) = 0;
        virtual void EnableFeature(String featureId, bool isEnabled) = 0;
    };
}

#endif
