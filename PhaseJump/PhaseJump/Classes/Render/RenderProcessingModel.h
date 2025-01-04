#pragma once

#include "RenderContextModel.h"
#include "RenderModel.h"
#include "SomeWorldSystem.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/7/24
 */
namespace PJ {
    class RenderProcessor;
    class RenderCameraModel;

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

        void ProcessPhase(String phase, RenderCameraModel* cameraModel);
    };
} // namespace PJ
