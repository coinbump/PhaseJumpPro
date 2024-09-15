#pragma once

#include "RenderProcessor.h"
#include "Utils.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 8/28/24
 */
namespace PJ {
    class CameraRenderModel;

    /// Batches render models by material to reduce the total # of draw calls and improve render
    /// performance
    class BatchByMaterialRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        // TODO: re-evaluate all registered render phases, probably don't want to run for each
        // camera
        BatchByMaterialRenderProcessor() :
            Base("Batch by material", { RenderPhase::Camera }) {}

        // MARK: RenderProcessor

        void Process(CameraRenderModel& cameraModel) override;
    };
} // namespace PJ
