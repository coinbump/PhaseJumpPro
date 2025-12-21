#pragma once

#include "RenderModel.h"
#include "VectorList.h"

/*
 RATING: 4 stars
 Tested and works
 cameras CODE REVIEW: 12/22/24
 */
namespace PJ {
    class RenderProcessor;
    class Renderer;
    class Camera;
    class SomeRenderContext;
    class WorldNode;

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
    class RenderPhaseModel {
    public:
        using This = RenderPhaseModel;

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
    class RenderCameraModel {
    public:
        using This = RenderCameraModel;
        using ModelSortFunc = std::function<bool(RenderModel const&, RenderModel const&)>;
        using NodeList = VectorList<WorldNode*>;

        struct Config {
            SomeRenderContext* context{};
            Camera& camera;
            VectorList<RenderModel>& renderModels;
            VectorList<WorldNode*>& nodes;
        };

        RenderPhaseModel phaseModel;

        /// Pointer, not reference for model copies
        SomeRenderContext* renderContext{};

        /// Camera being rendered
        Camera* camera{};

        /// Nodes to render
        NodeList& nodes;

        /// Models to render
        VectorList<RenderModel>& renderModels;

        /// Model groups (grouping should be done as a final step)
        // FUTURE: VectorList<RenderModelGroup> modelGroups;

        /// Temporary materials used by render processors
        /// Example: we can create a processor that shows mesh shape, collider bounds, etc
        VectorList<SP<RenderMaterial>> materials;

        /// Temporary renderers used by render processors
        /// We share memory from the renderer model, keep it in memory
        VectorList<SP<Renderer>> renderers;

        /// Materials created by render processor that override an existing material
        /// Multiple models might share the same material, this gives us a single source of truth
        UnorderedMap<RenderMaterial*, SP<RenderMaterial>> overrideMaterials;

        // MARK: Z-Order

        /// (Experimental). Used to sort render models for back-to-front render
        /// By default, standard Z-ordering is used (Breadth first order)
        ModelSortFunc modelSortFunc = [this](RenderModel const& lhs, RenderModel const& rhs) {
            // Z index layering allows us to render groups as layers
            if (lhs.zIndex < rhs.zIndex) {
                return true;
            }
            return lhs.order < rhs.order;
        };

        RenderCameraModel(Config const& config);

        /// @return Returns the override material for this model if one exists
        SP<RenderMaterial> OverrideMaterial(RenderModel const& model);

        /// Creates an override material for this model
        SP<RenderMaterial> MakeOverrideMaterial(RenderModel const& model);

        void SetPhase(String value);
    };
} // namespace PJ
