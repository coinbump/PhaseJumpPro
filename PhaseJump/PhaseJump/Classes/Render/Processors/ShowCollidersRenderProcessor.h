#pragma once

#include "Color.h"
#include "PropertyWrapper.h"
#include "RenderProcessor.h"
#include "RenderTypes.h"

/*
 RATING 4 stars
 Works, but limited functionality. Not optimized
 CODE REVIEW: 8/27/24
 */
namespace PJ {
    /// Renders a frame to show colliders (limited functionality)
    class ShowCollidersRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        //        PROPERTY_CONTAINER
        //        PROPERTY_COLOR(color);
        //        PropertyWrapper<Color> color2(propertyWrappers);
        Color color;

        // TODO: re-evaluate all registered render phases, probably don't want to run for each
        // camera
        ShowCollidersRenderProcessor(Color color = Color::blue) :
            Base("Show colliders", { RenderPhase::Camera }),
            color(color) {}

        // MARK: RenderProcessor

        void Process(CameraRenderModel& cameraModel) override;
    };
} // namespace PJ
