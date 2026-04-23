#pragma once

#include "StringUtils.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    class SomeRenderModel;
    struct StandardEdgeCore;
    template <class EdgeCore, class Core>
    class AcyclicGraphNode;

    /// DAG node type that owns a SomeRenderModel.
    using SomeRenderModelNode = AcyclicGraphNode<StandardEdgeCore, SP<SomeRenderModel>>;

    /// Names of common render model types
    namespace RenderModelType {
        /// Draw a mesh
        auto constexpr Draw = "draw";

        /// Push a stencil
        auto constexpr StencilPush = "stencil.push";

        /// Pop a stencil
        auto constexpr StencilPop = "stencil.pop";

        /// Viewport DAG boundary — binds the viewport camera before its child subtree renders
        auto constexpr Viewport = "viewport";

        /// Non-viewport camera DAG node — parents of the subtree rendered by that camera
        auto constexpr Camera = "camera";
    } // namespace RenderModelType

    /// Abstract base for render models dispatched by the render engine.
    ///
    /// The render pipeline builds a DAG of these for each render pass
    class SomeRenderModel {
    public:
        using This = SomeRenderModel;

        /// Type id
        String type;

        /// Node that owns this model
        SomeRenderModelNode* node{};

        virtual ~SomeRenderModel() = default;
    };
} // namespace PJ
