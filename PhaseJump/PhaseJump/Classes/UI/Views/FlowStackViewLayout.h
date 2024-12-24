#pragma once

#include "AlignFunc.h"
#include "Axis.h"
#include "LayoutInsets.h"
#include "SomeViewLayout.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/25/24
 */
namespace PJ {
    /// Arranges child views in a stack layout
    class FlowStackViewLayout : public ViewLayout {
    protected:
        Axis2D axis = Axis2D::X;
        float spacing{};
        AlignFunc alignFunc = AlignFuncs::center;

        // MARK: Editor

        VectorList<String> _editAlignOptions{ "Left", "Center", "Right" };
        std::optional<int> _editAlignOption;

        struct UnboundedInfo {
            float availableLength;
            VectorList<ViewProxy> boundedChildren;
            VectorList<ViewProxy> unboundedChildren;
        };

        bool isAxisUnbounded{};
        bool isOrthogonalUnbounded{};

        UnboundedInfo
        CalculateUnboundedInfo(Vector2 boundsSize, VectorList<ViewProxy> const& children);

        // MARK: ViewLayout

        void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;

    public:
        using Base = ViewLayout;
        using This = FlowStackViewLayout;

        FlowStackViewLayout(
            Axis2D axis, float spacing = 0, AlignFunc alignFunc = AlignFuncs::center
        );

        Axis2D Axis() const {
            return axis;
        }

        void SetSpacing(float value);
        void SetAlignFunc(AlignFunc value);
        void SetAxis(Axis2D value);

        // MARK: SomeViewLayout:

        Vector2 ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) override;

        String TypeName() const override {
            return axis == Axis2D::X ? "HStackViewLayout" : "VStackViewLayout";
        }
    };
} // namespace PJ
