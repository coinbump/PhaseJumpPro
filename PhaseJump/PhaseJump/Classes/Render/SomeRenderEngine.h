#pragma once

#include "Base.h"
#include "List.h"
#include "UnorderedSet.h"
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
    class RenderMaterial;
    class SomeRenderContext;

    /// Standard render phases: start -> preProcess -> processs -> postProcess
    namespace RenderPhaseId {
        auto constexpr Start = "start";
        auto constexpr PreProcess = "preProcess";
        auto constexpr PostProcess = "postProcess";
    } // namespace RenderPhaseId

    struct RenderDrawModel {
        VectorList<RenderModel> const& models;
    };

    /// A render engine should be able to perform standard render operations
    class SomeRenderEngine : public Base {
    public:
        /// If true, render states won't be switched if we're already in that state
        /// Flag enables us to A/B benchmark with this on/off
        bool optimizeStateSwitches = true;

        virtual void RenderStart(SomeRenderContext* context) = 0;
        virtual void RenderDraw(RenderDrawModel const& drawModel) = 0;

        virtual void EnableFeature(String featureId, bool isEnabled) = 0;

        virtual void ProjectionMatrixLoadOrthographic(Vector2 size) = 0;
        virtual void LoadTranslate(Vector3 value) = 0;

        virtual SP<SomeRenderContext> MakeTextureBuffer() = 0;
        virtual UnorderedSet<String> EnabledFeatures() = 0;

        /// Called once for each render pass
        virtual void RenderPassStart() = 0;
        virtual bool IsContextCleared(uint32_t id) = 0;
        virtual void SetIsContextCleared(uint32_t id, bool value) = 0;
    };
} // namespace PJ
