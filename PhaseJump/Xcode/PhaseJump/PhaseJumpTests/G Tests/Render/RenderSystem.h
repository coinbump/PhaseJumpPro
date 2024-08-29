#pragma once

#include "SomeWorldSystem.h"
#include "RenderContextModel.h"
#include "RenderModel.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/28/24
 */
namespace PJ {
    class RenderProcessor;

    /// Common ids for render model groups
    namespace RenderModelGroupId {
        static auto const Opaque = "opaque";
        static auto const Blend = "blend";
    } // namespace RenderModelGroupId

    /// Allows render models to be grouped (Example: group blend and opaque renders separately)
    struct RenderModelGroup {
        String id;
        VectorList<RenderModel> value;
    };

    /// Defines the model for a render pass
    /// The render system sends this to render processors, which can then modify it to
    /// add new draw commands, modify existing commands, filter nodes or cameras, etc.
    struct RenderSystemModel {
        /// Pointer, not reference for model copies
        SomeRenderContext* renderContext = nullptr;

        /// Nodes to be rendered
        VectorList<WorldNode*> nodes;

        /// Cameras to render
        VectorList<SomeCamera*> cameras;

        /// Models to render
        VectorList<RenderModel> models;

        /// Model groups (grouping should be done as a final step)
        VectorList<RenderModelGroup> modelGroups;

        /// Materials used by render processors
        /// Example: mesh renders for debug render processor that shows vertices/bounds/colliders/etc.
        VectorList<SP<RenderMaterial>> materials;

        /// Mapped materials used by render processors
        /// Allows multiple processors to share a material
        UnorderedMap<String, SP<RenderMaterial>> materialsMap;

        RenderSystemModel(RenderContextModel& contextModel) : renderContext(contextModel.renderContext), nodes(contextModel.nodes), cameras(contextModel.cameras) {
        }
    };

    class RenderContextModel;

    /// Render pipeline logic for the world. Every world needs a render system
    class RenderSystem : public SomeWorldSystem {
    public:
        using ProcessorList = VectorList<SP<RenderProcessor>>;

    protected:
        ProcessorList processors;

    public:
        RenderSystem() {}
        virtual ~RenderSystem() {}

        void Add(SP<RenderProcessor> processor) {
            processors.push_back(processor);
        }

        ProcessorList& Processors() { return processors; }
        ProcessorList const& Processors() const { return processors; }

        // MARK: SomeWorldSystem

        std::optional<RenderResult> Render(RenderContextModel& contextModel) override;
    };
}
