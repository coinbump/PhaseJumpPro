#pragma once

#include "MaterialRenderModel.h"
#include "SomeRenderModel.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    /// Pops the most recent stencil push. Ends the clipping scope opened by a matching
    /// StencilPushRenderModel.
    class StencilPopRenderModel : public SomeRenderModel {
    public:
        using Base = SomeRenderModel;
        using This = StencilPopRenderModel;

        StencilPopRenderModel() {
            type = RenderModelType::StencilPop;
        }
    };
} // namespace PJ
