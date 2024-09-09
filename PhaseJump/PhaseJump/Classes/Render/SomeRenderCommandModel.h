#pragma once

#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/17/24
 */
namespace PJ {
    /// A command that represents a render intent, but is not backed by a
    /// concrete implementation It is up to the render engine to interpret proxy
    /// commands appropriately
    struct SomeRenderCommandModel {
        String id;

        SomeRenderCommandModel(String id) :
            id(id) {}

        virtual ~SomeRenderCommandModel() {}
    };

    template <class Core>
    struct RenderCommandModel : public SomeRenderCommandModel {
        using Base = SomeRenderCommandModel;

        Core core;

        RenderCommandModel(String id, Core core = Core()) :
            Base(id),
            core(core) {}
    };

    namespace RenderCommandId {
        auto constexpr ProjectionMatrixLoadOrthographic = "projection.matrix.load.orthographic";
        auto constexpr LoadTranslate = "load.translate";
    } // namespace RenderCommandId

    namespace RenderCommandModels {
        // TODO: should these be SP? Since ownership isn't clear?
        UP<RenderCommandModel<Vector2>> ProjectionMatrixLoadOrthographic(Vector2 size);
        UP<RenderCommandModel<Vector3>> LoadTranslate(Vector3 value);
    } // namespace RenderCommandModels
} // namespace PJ
