#pragma once

#include "RenderTypes.h"
#include "UnorderedSet.h"
#include "Utils.h"
#include <functional>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class RenderWorldSystem;
    class RenderCameraModel;

    /// A processor in the render pipeline
    /// Each processor creates render models, or performs operations on models created by previous
    /// processors
    class RenderProcessor {
    protected:
        bool isEnabled = true;

    public:
        /// The system that owns this processor, if any
        RenderWorldSystem* system{};

        /// Called for system render phases
        using ProcessFunc = std::function<void(String phase)>;

        /// Called for camera render phases
        using ProcessCameraFunc = std::function<void(RenderCameraModel&)>;

        /// Use facing name, for browsing
        String name;

        /// Called for system render phases
        ProcessFunc processFunc;

        /// Called for camera render phases
        ProcessCameraFunc processCameraFunc;

        /// Each processor must register to run for specific render phases
        UnorderedSet<String> phases;

        struct Config {
            String name;
            UnorderedSet<String> phases;
        };

        RenderProcessor(Config config) :
            name(config.name),
            phases(config.phases) {}

        virtual ~RenderProcessor() {}

        bool IsEnabled() const {
            return isEnabled;
        }

        void Enable(bool value) {
            this->isEnabled = value;
        }

        virtual void Process(String phase) {
            GUARD(processFunc)
            processFunc(phase);
        }

        virtual void Process(RenderCameraModel& cameraModel) {
            GUARD(processCameraFunc)
            processCameraFunc(cameraModel);
        }
    };
} // namespace PJ
