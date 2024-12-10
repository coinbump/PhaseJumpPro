#pragma once

#include "Macros.h"
#include "SomeViewLayout.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 11/29/24
 */
namespace PJ {
    /// Matches the size of the views to another view
    /// Used for background and overlay views
    class MatchSizeViewLayout : public ViewLayout {
    public:
        // FUTURE: evaluate MatchZStackViewLayout if needed
        using Base = ViewLayout;
        using This = MatchSizeViewLayout;

    protected:
        WP<View2D> matchView;
        std::optional<Vector2> matchSize;

        // MARK: ViewLayout

        void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;

        void StartLayout() override {
            ViewLayout::StartLayout();

            matchSize = {};
        }

    public:
        MatchSizeViewLayout(SP<View2D> matchView) :
            matchView(matchView) {}

        // MARK: SomeViewLayout:

        Vector2 ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) override;

        String TypeName() const override {
            return "MatchSizeViewLayout";
        }
    };
} // namespace PJ
