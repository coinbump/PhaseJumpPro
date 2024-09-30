#pragma once

#include "RenderContextModel.h"
#include "RenderModel.h"
#include "RenderProcessingModel.h"
#include "SomeWorldSystem.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/28/23
 */
namespace PJ {
    class RenderProcessor;
    class SomeRenderer;

    /// Common ids for render model groups
    namespace RenderModelGroupId {
        auto constexpr Opaque = "opaque";
        auto constexpr Blend = "blend";
    } // namespace RenderModelGroupId

    /// Allows render models to be grouped (Example: group blend and opaque renders separately)
    struct RenderModelGroup {
        String id;
        VectorList<RenderModel> value;
    };

    /// Manages render phases and notification when phase changes
    class PhaseRenderModel {
    public:
        using This = PhaseRenderModel;

        using OnPhaseChangeFunc = std::function<void(This&, String phase)>;

        OnPhaseChangeFunc onPhaseChangeFunc;

    protected:
        /// The current render phase
        String phase;

    public:
        void SetPhase(String value);
    };

    /// Defines the model for a render pass
    /// The render system sends this to render processors, which can then modify it to
    /// add new draw commands, modify existing commands, filter nodes or cameras, etc.
    class CameraRenderModel {
    public:
        using This = CameraRenderModel;

    public:
        using ModelSortFunc = std::function<bool(RenderModel const&, RenderModel const&)>;

        PhaseRenderModel phaseModel;

        /// Pointer, not reference for model copies
        SomeRenderContext* renderContext = nullptr;

        /// Root node, used for z-order sorting
        WorldNode* root = nullptr;

        /// Nodes to be rendered
        VectorList<WorldNode*> nodes;

        /// Camera being rendered
        SomeCamera* camera = nullptr;

        /// Models to render
        VectorList<RenderModel> models;

        /// Model groups (grouping should be done as a final step)
        VectorList<RenderModelGroup> modelGroups;

        /// Temporary materials used by render processors
        /// Example: we can create a processor that shows mesh shape, collider bounds, etc
        VectorList<SP<RenderMaterial>> materials;

        /// Temporary renderers used by render processors
        /// Because we share memory from the renderer colors, we need to store it somewhere
        VectorList<SP<SomeRenderer>> renderers;

        /// Materials created by render processor that override an existing material
        /// Multiple models might share the same material, this gives us a single source of truth
        UnorderedMap<RenderMaterial*, SP<RenderMaterial>> overrideMaterials;

        // MARK: Z-Order

        /// Used to sort render models for back-to-front render
        ModelSortFunc modelSortFunc = [this](RenderModel const& lhs, RenderModel const& rhs) {
            // Z index layering allows us to render groups as layers
            if (lhs.zIndex < rhs.zIndex) {
                return true;
            }
            return lhs.order < rhs.order;
        };

        /// @return Returns the override material for this model if one exists
        SP<RenderMaterial> OverrideMaterial(RenderModel const& model) {
            // TODO: unit test
            auto i = overrideMaterials.find(model.BaseMaterial());
            GUARDR(i != overrideMaterials.end(), nullptr)

            return i->second;
        }

        /// Creates an override material for this model
        SP<RenderMaterial> MakeOverrideMaterial(RenderModel const& model) {
            // TODO: unit test
            auto baseMaterial = model.BaseMaterial();
            GUARDR(baseMaterial, nullptr)

            auto i = overrideMaterials.find(baseMaterial);
            GUARDR(i == overrideMaterials.end(), i->second)

            auto result = MAKE<RenderMaterial>();
            *result = *baseMaterial;
            overrideMaterials.insert_or_assign(baseMaterial, result);

            return result;
        }

        void SetPhase(String value);

        CameraRenderModel(
            RenderContextModel& contextModel, SomeCamera& camera, VectorList<RenderModel> models
        );
    };

    class RenderContextModel;

    /// Render pipeline logic for the world. Every world needs a render system
    class RenderWorldSystem : public SomeWorldSystem {
    public:
        class Model {
        public:
            PhaseRenderModel phaseModel;
            RenderProcessingModel processingModel;
        };

        using Base = SomeWorldSystem;
        using ProcessorList = RenderProcessingModel::ProcessorList;

    protected:
        Model model;

    public:
        RenderWorldSystem(String name = "Render") :
            Base(name) {}

        virtual ~RenderWorldSystem() {}

        void Add(SP<RenderProcessor> processor);

        ProcessorList const& Processors() const;

        // MARK: SomeWorldSystem

        std::optional<RenderResult> Render(RenderContextModel& contextModel) override;
    };
} // namespace PJ
