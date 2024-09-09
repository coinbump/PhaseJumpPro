#pragma once

#include "Color.h"
#include "PropertyWrapper.h"
#include "RenderProcessor.h"

/*
 RATING 4 stars
 Works, but limited functionality. Not optimized
 CODE REVIEW: 8/27/24
 */
namespace PJ {
    /// Renders a frame to show colliders (limited functionality)
    class ShowCollidersRenderProcessor : public RenderProcessor {
    public:
        //        PROPERTY_CONTAINER
        //        PROPERTY_COLOR(color);
        //        PropertyWrapper<Color> color2(propertyWrappers);
        Color color;

        ShowCollidersRenderProcessor(Color color = Color::blue) :
            color(color) {
            this->name = "Show colliders";
        }

        // MARK: RenderProcessor

        void Process(RenderSystemModel& systemModel) override;
    };
} // namespace PJ
