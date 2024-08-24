#pragma once

#include "Base.h"
#include "List.h"
#include "Vector2.h"
#include "Vector3.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    struct RenderModel;
    struct RenderContextModel;
    class RenderProcessModel;
    class RenderMaterial;

    /// Standard render phases: start -> preProcess -> processs -> postProcess
    namespace RenderPhaseId {
        static auto const Start = "start";
        static auto const PreProcess = "preProcess";
        static auto const PostProcess = "postProcess";
    } // namespace RenderPhaseId

    using RenderProcessorFunc =
        std::function<RenderProcessModel(RenderContextModel const&, RenderProcessModel&)>;

    struct RenderPhase {
        String id;
        VectorList<RenderProcessorFunc> processorFuncs;
    };

    struct RenderPipeline {
        List<RenderPhase> phases;
    };

    struct RenderProcessModel {
        VectorList<RenderModel> renderModels;

        /// Temporary materials used by debug render processors
        VectorList<SP<RenderMaterial>> tempMaterials;
    };

    /// A render engine should be able to perform standard render operations
    class SomeRenderEngine : public Base {
    public:
        virtual void RenderStart(RenderContextModel const& contextModel) = 0;
        virtual void RenderProcess(RenderProcessModel const& processModel) = 0;
        virtual void RenderDraw() = 0;

        virtual void SetLineWidth(float lineWidth) = 0;
        virtual void EnableFeature(String featureId, bool isEnabled) = 0;

        virtual void ProjectionMatrixLoadOrthographic(Vector2 size) = 0;
        virtual void LoadTranslate(Vector3 value) = 0;
    };
} // namespace PJ
