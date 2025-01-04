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

        using ProcessFunc = std::function<void(String phase, RenderCameraModel*)>;

        /// Use facing name, for browsing
        String name;

        /// Called for render phases
        ProcessFunc processFunc;

        /// Each processor must register to run for specific render phases
        UnorderedSet<String> phases;

        struct Config {
            String name;
            UnorderedSet<String> phases;
        };

        struct Phase {
            /// Phase id (render start, render draw, etc.)
            String id;

            /// Camera model if we're processing for a specific camera
            RenderCameraModel* cameraModel{};
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

        virtual void Process(Phase phase) {
            if (processFunc) {
                processFunc(phase.id, phase.cameraModel);
            }

            if (phase.cameraModel) {
                Process(*phase.cameraModel);
            }
        }

        virtual void Process(RenderCameraModel& cameraModel) {}
    };
} // namespace PJ
