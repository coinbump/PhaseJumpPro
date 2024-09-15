#pragma once

#include "RenderTypes.h"
#include "UnorderedSet.h"
#include "Utils.h"
#include <functional>

// CODE REVIEW: ?/23
namespace PJ {
    class RenderWorldSystem;
    class CameraRenderModel;

    /// A node in the render pipeline
    /// Each processor creates render models, or performs operations on models created by previous
    /// processors
    class RenderProcessor {
    protected:
        bool isEnabled = true;

    public:
        /// Called for system render phases
        using ProcessFunc = std::function<void(String phase)>;

        /// Called for camera render phases
        using ProcessCameraFunc = std::function<void(CameraRenderModel&)>;

        String name;
        ProcessFunc processFunc;
        ProcessCameraFunc processCameraFunc;

        /// Each processor must register to run for specific render phases
        UnorderedSet<String> phases;

        RenderProcessor(String name, std::initializer_list<String> phases) :
            name(name),
            phases(phases) {}

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

        virtual void Process(CameraRenderModel& cameraModel) {
            GUARD(processCameraFunc)
            processCameraFunc(cameraModel);
        }

#if FALSE
        class SomeRenderSystemCommand {};

        namespace SomeRenderSystemCommands {
            // ??? What advantage do we gain by this? Just let them alter the render system directly
            class AddModelsCommand {
                // modelGroups here
            };

            class ReplaceModelsCommand {
                // modelGroups here
            };

            class FilterNodesCommand {};

            class FilterCamerasCommand {};

            class
        }; // namespace SomeRenderSystemCommands

        virtual SP<List<SomeRenderSystemCommand>> ProcessV2(CameraRenderModel const& cameraModel) {
            // How do we do this by returning a value and not modifying the render system directly?
            // A: commands. Problem: limited, not sure what all the possibilities are
            // B: Copy render system model in processor. Cons: expensive? Each processor has to copy
            // everything C: allow render processor to directly edit context model and render system
            // <- fragile??

            //            for (auto& group: renderSystem.modelGroups) {
            //                ProcessModelGroup(renderSystem, )
            //            }

            GUARDR(processFunc, nullptr)
            return processFunc(renderSystem, contextModel);
        }
#endif
    };
} // namespace PJ
