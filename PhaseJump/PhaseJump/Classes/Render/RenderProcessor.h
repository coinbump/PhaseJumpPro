#pragma once

#include "Utils.h"
#include <functional>

// CODE REVIEW: ?/23
namespace PJ {
    class RenderWorldSystem;
    class RenderSystemModel;

    /// A node in the render pipeline
    /// Each processor creates render models, or performs operations on models created by previous
    /// processors
    class RenderProcessor {
    protected:
        bool isEnabled = true;

    public:
        using ProcessFunc = std::function<void(RenderSystemModel&)>;

        String name;
        ProcessFunc processFunc;

        RenderProcessor(String name = "") :
            name(name) {}

        virtual ~RenderProcessor() {}

        bool IsEnabled() const {
            return isEnabled;
        }

        void Enable(bool value) {
            this->isEnabled = value;
        }

        virtual void Process(RenderSystemModel& systemModel) {
            GUARD(processFunc)
            processFunc(systemModel);
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

        virtual SP<List<SomeRenderSystemCommand>> ProcessV2(RenderSystemModel const& systemModel) {
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
