#pragma once

#include "Base.h"
#include "List.h"
#include "SomeRenderModel.h"
#include "Tags.h"
#include "UnorderedSet.h"
#include "Vector2.h"
#include "Vector3.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 1/11/25
 */
namespace PJ {
    struct MaterialRenderModel;
    struct RenderContextModel;
    class RenderMaterial;
    class RenderPassModel;
    class SomeRenderContext;
    class StencilPushRenderModel;

    /// Whether a render context should clear itself when its `Clear()` is invoked
    enum class RenderClearType {
        /// Clear to the context's clear color each pass
        Clear,

        /// Skip clearing — preserves previous contents (for accumulation, e.g. paint buffers)
        None
    };

    /**
     Accepts render models to be rendered.
     */
    class SomeRenderEngine : public Base {
    public:
        /// Default clear behavior
        RenderClearType clearType = RenderClearType::Clear;

        /// Called before RenderDraw
        virtual void RenderStart(SomeRenderContext* context) = 0;

        /// Walk the render-pass DAG in DFS pre-order and dispatch each node by type.
        virtual void RenderDraw(RenderPassModel const& pass) {}

        /// Walk a camera subtree of the render pass DAG. Camera is root node
        virtual void RenderDrawSubtree(SomeRenderModelNode const& startNode) {}

        /// Push a stencil region. Subsequent draws are clipped to the area defined by
        /// models mesh (transformed by model.matrix) until the matching pop.
        virtual void RenderStencilPush(StencilPushRenderModel const& model) {}

        /// Pop the most recent stencil region, restoring the previous clipping scope.
        virtual void RenderStencilPop() {}

        /// Enables or disables a feature by feature id
        virtual void EnableFeature(String featureId, bool isEnabled) = 0;

        /// Loads an orthographic projection matrix
        virtual void ProjectionMatrixLoadOrthographic(Vector2 size) = 0;

        /// Loads a perspective projection matrix.
        /// fovRadians is the vertical field of view; aspect is width/height.
        virtual void ProjectionMatrixLoadPerspective(
            float fovRadians, float aspect, float zNear, float zFar
        ) = 0;

        /// Load a translation view matrix
        virtual void LoadTranslate(Vector3 value) = 0;

        /// Create a render context for an offscreen texture buffer
        virtual SP<SomeRenderContext> MakeTextureBuffer() = 0;

        /// @return Returns a list of enabled feature ids
        virtual UnorderedSet<String> EnabledFeatures() = 0;

        /**
         Called once, at the beginning of each render pass

         A render pass renders each of the cameras
         */
        virtual void RenderPassStart() = 0;

        /// @return Returns true if the context has been cleared.
        /// Useful if multiple cameras share the same context
        virtual bool IsContextCleared(uint32_t id) = 0;

        /// Sets a flag to indicate the context has been cleared
        virtual void SetIsContextCleared(uint32_t id, bool value) = 0;

        /// @return Returns diagnostic info for the editor. Each Tags entry carries a "label" and
        /// "value" string pair.
        virtual VectorList<Tags> EditorInfoList() {
            return {};
        }
    };
} // namespace PJ
