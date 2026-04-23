#pragma once

#include "RenderTypes.h"
#include "SomeRenderModel.h"
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
        struct Config {
            String name;
            UnorderedSet<String> phases;
        };

        struct Phase {
            /// Phase id (render start, render draw, etc.)
            String id;

            /// Render node for the processor. For a system-wide processor it is
            /// the entire tree (children are per-camera render subtrees). For a camera-
            /// specific processor it is the camera's node (children are the camera's
            /// render models)
            SomeRenderModelNode* rootNode{};

            bool IsCamera() const;
        };

        /// The system that owns this processor, if any
        RenderWorldSystem* system{};

        using ProcessFunc = std::function<void(Phase const& phase)>;

        /// Use facing name, for browsing
        String name;

        /// Called for render phases
        ProcessFunc processFunc;

        /// Each processor must register to run for specific render phases
        UnorderedSet<String> phases;

        RenderProcessor(Config const& config) :
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
                processFunc(phase);
            }

            if (phase.rootNode) {
                Process(*phase.rootNode);
            }
        }

        /// Called with the render model root. Processor can walk the children of root and
        /// add, modify, or replace nodes as needed
        virtual void Process(SomeRenderModelNode& rootNode);
    };
} // namespace PJ
