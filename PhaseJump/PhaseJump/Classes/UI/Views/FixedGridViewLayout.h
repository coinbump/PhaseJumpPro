#pragma once

#include "SomeAligner.h"
#include "SomeViewLayout.h"

namespace PJ {
    // TODO: work in progress
    /// Arranges child views in a (x by y) grid sized layout, where each cell is a fixed size,
    /// and all cells are equal size
    class FixedGridViewLayout : public ViewLayout {
    public:
        using Base = ViewLayout;
        using This = FixedGridViewLayout;

        struct Config {
            // TODO: should this be width?
            Vector2Int gridSize{ 2, 2 };

            /// Defines how content is aligned inside the grid cell
            AlignFunc xAlignFunc = AlignFuncs::center;

            /// Defines how content is aligned inside the grid cell
            AlignFunc yAlignFunc = AlignFuncs::center;

            Vector2 spacing;
        };

    protected:
        Config config;

        // MARK: ViewLayout

        void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;

    public:
        FixedGridViewLayout(Config config);

        // MARK: SomeViewLayout:

        Vector2 ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) override;

        String TypeName() const override {
            return "FixedGridViewLayout";
        }
    };
} // namespace PJ
