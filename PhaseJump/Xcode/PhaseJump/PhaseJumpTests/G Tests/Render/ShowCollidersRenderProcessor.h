#pragma once

#include "RenderProcessor.h"
#include "Color.h"

/*
 RATING 4 stars
 Works, but limited functionality. Not optimized (slow)
 CODE REVIEW: 8/27/24
 */
namespace PJ {
    /// Renders a frame to show colliders (limited functionality)
    class ShowCollidersRenderProcessor : public RenderProcessor {
    public:
        Color color;

        ShowCollidersRenderProcessor(Color color = Color::blue) : color(color) {
            this->name = "Show colliders";
        }
        
        // MARK: RenderProcessor
        
        void Process(RenderSystemModel& systemModel) override;
    };
}
