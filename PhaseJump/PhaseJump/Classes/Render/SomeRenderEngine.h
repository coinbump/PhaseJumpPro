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
 CODE REVIEW: 1/11/25
 */
namespace PJ {
    struct RenderModel;
    struct RenderContextModel;
    class RenderMaterial;
    class SomeRenderContext;

    /// Stores render models for a render draw
    struct RenderDrawModel {
        VectorList<RenderModel> const& renderModels;
    };

    /**
     Accepts render models to be rendered.
     */
    class SomeRenderEngine : public Base {
    public:
        /// Called before RenderDraw
        virtual void RenderStart(SomeRenderContext* context) = 0;

        /// Called to render the render models
        virtual void RenderDraw(RenderDrawModel const& drawModel) = 0;

        /// Enables or disables a feature by feature id
        virtual void EnableFeature(String featureId, bool isEnabled) = 0;

        /// Loads an orthographic projection matrix
        virtual void ProjectionMatrixLoadOrthographic(Vector2 size) = 0;

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
    };
} // namespace PJ
