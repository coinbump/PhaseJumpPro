#pragma once

#include "AlignFunc.h"
#include "LayoutInsets.h"
#include "SomeViewLayout.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/25/24
 */
namespace PJ {
    /// Arranges child views in a Z-order stack layout
    class ZStackViewLayout : public ViewLayout {
    protected:
        bool isWidthUnbounded{};
        bool isHeightUnbounded{};
        float maxChildWidth{};
        float maxChildHeight{};

        // MARK: ViewLayout

        void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;

    public:
        using Base = ViewLayout;
        using This = ZStackViewLayout;

        AlignFunc xAlignFunc = AlignFuncs::center;
        AlignFunc yAlignFunc = AlignFuncs::center;

        ZStackViewLayout(
            AlignFunc xAlignFunc = AlignFuncs::center, AlignFunc yAlignFunc = AlignFuncs::center
        ) :
            xAlignFunc(xAlignFunc),
            yAlignFunc(yAlignFunc) {}

        // MARK: SomeViewLayout:

        Vector2 ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) override;

        String TypeName() const override {
            return "ZStackViewLayout";
        }
    };
} // namespace PJ
