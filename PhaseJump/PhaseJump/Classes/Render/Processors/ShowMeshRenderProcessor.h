#pragma once

#include "Color.h"
#include "PropertyWrapper.h"
#include "RenderProcessor.h"

/*
 RATING 4 stars
 Works, but limited functionality. Not optimized
 CODE REVIEW: 8/31/24
 */
namespace PJ {
    /// Replaces rendered meshes with a mesh shape render
    class ShowMeshRenderProcessor : public RenderProcessor {
    public:
        Color color;

        ShowMeshRenderProcessor(Color color = Color::blue) :
            color(color) {
            this->name = "Show mesh";
        }

        // MARK: RenderProcessor

        void Process(RenderSystemModel& systemModel) override;
    };
} // namespace PJ
