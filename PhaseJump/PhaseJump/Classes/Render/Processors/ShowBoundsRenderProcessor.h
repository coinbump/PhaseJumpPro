#pragma once

#include "Color.h"
#include "PropertyMirror.h"
#include "RenderProcessor.h"

/*
 RATING 4 stars
 Works, but limited functionality
 CODE REVIEW: 8/27/24
 */
namespace PJ {
    /// Renders a frame to show bounds
    class ShowBoundsRenderProcessor : public RenderProcessor {
    public:
        Color color;

        ShowBoundsRenderProcessor(Color color = Color::green) :
            color(color) {
            this->name = "Show bounds";
        }

        // MARK: RenderProcessor

        void Process(RenderSystemModel& systemModel) override;
    };
} // namespace PJ
