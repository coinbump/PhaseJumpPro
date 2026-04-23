#pragma once

#include "MaterialRenderModel.h"
#include "SomeRenderModel.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    class Camera;

    /// DAG node marking a viewport boundary. Its children in the DAG are the models
    /// rendered inside the viewport's camera context.
    ///
    /// Viewport boundaries also stop feature-inheritance walks (e.g. blend-mode override
    /// does not cross into a viewport's subtree).
    class ViewportRenderModel : public SomeRenderModel {
    public:
        using Base = SomeRenderModel;
        using This = ViewportRenderModel;

        Camera* viewportCamera{};

        ViewportRenderModel() {
            type = RenderModelType::Viewport;
        }
    };
} // namespace PJ
