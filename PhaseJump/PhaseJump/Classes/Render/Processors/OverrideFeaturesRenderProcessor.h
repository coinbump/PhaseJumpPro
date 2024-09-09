#pragma once

#include "RenderProcessor.h"
#include "RenderTypes.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Tested and works
 9/1/24
 */
namespace PJ {
    /// Overrides enabled state for render features in existing render models
    /// Useful to enforce Z-ordering or batching
    class OverrideFeaturesRenderProcessor : public RenderProcessor {
    public:
        // FUTURE: Create override material processor if needed
        UnorderedMap<String, RenderFeatureState> map;

        OverrideFeaturesRenderProcessor() {
            this->name = "Override features";
        }

        // MARK: RenderProcessor

        void Process(RenderSystemModel& systemModel) override;
    };
} // namespace PJ
