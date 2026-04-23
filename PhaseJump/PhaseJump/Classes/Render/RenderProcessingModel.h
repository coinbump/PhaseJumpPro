#pragma once

#include "MaterialRenderModel.h"
#include "RenderContextModel.h"
#include "SomeWorldSystem.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/7/24
 */
#include "SomeRenderModel.h"

namespace PJ {
    class RenderProcessor;

    /// Model for a render processing pipeline
    /// Multiple objects can have their own pipeline (system, camera)
    class RenderProcessingModel {
    public:
        using ProcessorList = VectorList<SP<RenderProcessor>>;

    protected:
        ProcessorList processors;

    public:
        void Add(SP<RenderProcessor> processor) {
            processors.push_back(processor);
        }

        ProcessorList const& Processors() const {
            return processors;
        }

        /// Dispatches the phase to every processor. `rootNode` is the processor scope —
        /// pass root for system-wide processors, camera DAG node for camera-specific ones.
        void ProcessPhase(String phase, SomeRenderModelNode* rootNode);
    };
} // namespace PJ
