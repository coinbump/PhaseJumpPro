#pragma once

#include "RenderCameraModel.h"
#include "RenderContextModel.h"
#include "RenderModel.h"
#include "RenderProcessingModel.h"
#include "SomeWorldSystem.h"

/*
 RATING: 4 stars
 Tested and works. Missing full support for multiple cameras, offscreen view ports, and perspective
 cameras CODE REVIEW: 12/22/24
 */
namespace PJ {
    class RenderContextModel;

    /// Render pipeline logic for the world. Every world needs a render system
    class RenderWorldSystem : public SomeWorldSystem {
    public:
        class Model {
        public:
            RenderPhaseModel phaseModel;
            RenderProcessingModel processingModel;
        };

        using Base = SomeWorldSystem;
        using ProcessorList = RenderProcessingModel::ProcessorList;

    protected:
        Model model;

    public:
        RenderWorldSystem(String name = "Render");

        virtual ~RenderWorldSystem() {}

        void Add(SP<RenderProcessor> processor);

        ProcessorList const& Processors() const;

        // MARK: SomeWorldSystem

        std::optional<RenderResult> Render(RenderContextModel& contextModel) override;
    };
} // namespace PJ
