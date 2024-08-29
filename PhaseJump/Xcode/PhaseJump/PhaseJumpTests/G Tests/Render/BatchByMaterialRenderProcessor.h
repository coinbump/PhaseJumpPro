#pragma once

#include "RenderProcessor.h"
#include "Utils.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 8/28/24
 */
namespace PJ {
    class RenderSystemModel;
    
    /// Batches render models by material to reduce the total # of draw calls and improve render performance
    class BatchByMaterialRenderProcessor : public RenderProcessor {
    public:
        BatchByMaterialRenderProcessor() {
            this->name = "Batch by material";
        }

        // MARK: RenderProcessor

        void Process(RenderSystemModel& systemModel) override;
    };
}
