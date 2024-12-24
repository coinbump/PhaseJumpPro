#pragma once

#include "RenderProcessor.h"
#include "RenderTypes.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    /// Overrides enabled state for render features in existing render models
    /// Useful to enforce Z-ordering or batching
    class OverrideFeaturesRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        // FUTURE: Create override material processor if needed
        UnorderedMap<String, RenderFeatureState> map;

        // TODO: re-evaluate all registered render phases, probably don't want to run for each
        // camera
        OverrideFeaturesRenderProcessor() :
            Base({ .name = "Override features", .phases = { RenderPhase::Camera } }) {}

        // MARK: RenderProcessor

        void Process(RenderCameraModel& cameraModel) override;
    };
} // namespace PJ
