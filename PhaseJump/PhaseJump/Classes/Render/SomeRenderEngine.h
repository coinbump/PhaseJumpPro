#ifndef PJSOMERENDERENGINE_H
#define PJSOMERENDERENGINE_H

#include "Base.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    struct RenderModel;
    struct RenderContextModel;

    namespace RenderPhase {
        static auto const Start = "start";
        static auto const PreProcess = "preProcess";
        static auto const PostProcess = "postProcess";
    } // namespace RenderPhase

    /// A render engine should be able to perform standard render operations
    class SomeRenderEngine : public Base {
    public:
        virtual void RenderStart(RenderContextModel const& contextModel) = 0;
        virtual void RenderProcess(RenderModel const& model) = 0;
        virtual void RenderDraw() = 0;

        virtual void SetLineWidth(float lineWidth) = 0;
        virtual void EnableFeature(String featureId, bool isEnabled) = 0;
    };
} // namespace PJ

#endif
