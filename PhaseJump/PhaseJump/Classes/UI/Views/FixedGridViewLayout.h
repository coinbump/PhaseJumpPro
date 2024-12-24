#pragma once

#include "AlignFunc.h"
#include "Alignment2D.h"
#include "SomeViewLayout.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/15/24
 */
namespace PJ {
    /**
     Basic fixed grid layout

     Arranges child views in a (x by y) grid sized layout, where each cell is a fixed size,
     all cells are equal size, and the grid has a predetermined number of columns and rows
     */
    class FixedGridViewLayout : public ViewLayout {
    protected:
        Vector2Int gridSize{ 2, 2 };
        Vector2 cellSize{ 100, 100 };

        /// Defines how content is aligned in each cell
        Alignment2D alignment;

        /// Spacing between rows/columns
        Vector2 spacing;

    public:
        using Base = ViewLayout;
        using This = FixedGridViewLayout;

        struct Config {
            Vector2Int gridSize{ 2, 2 };
            Vector2 cellSize{ 100, 100 };

            /// Defines how content is aligned in each cell
            Alignment2D alignment;

            /// Spacing between rows/columns
            Vector2 spacing;
        };

    protected:
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
