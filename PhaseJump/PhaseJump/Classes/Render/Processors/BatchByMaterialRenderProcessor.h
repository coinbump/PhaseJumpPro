#pragma once

#include "RenderProcessor.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    class RenderCameraModel;
    class RenderModel;

    /// Batches render models by material to reduce the total # of draw calls and improve render
    /// performance
    class BatchByMaterialRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        BatchByMaterialRenderProcessor() :
            Base({ .name = "Batch by material", .phases = { RenderPhaseId::DrawPrepare } }) {}

        std::optional<RenderModel> Combine(VectorList<RenderModel*>& renderModels);

        // MARK: RenderProcessor

        void Process(RenderCameraModel& cameraModel) override;
    };
} // namespace PJ
