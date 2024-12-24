#pragma once

#include "LayoutInsets.h"
#include "SomeViewLayout.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/25/24
 */
namespace PJ {
    class PadViewLayout : public ViewLayout {
    protected:
        LayoutInsets insets;

    public:
        PadViewLayout(LayoutInsets insets);

        void SetInsets(LayoutInsets value);

        void SetLeadingInset(float value) {
            LayoutInsets newValue = insets;
            newValue.leading = value;
            SetInsets(newValue);
        }

        void SetTrailingInset(float value) {
            LayoutInsets newValue = insets;
            newValue.trailing = value;
            SetInsets(newValue);
        }

        void SetTopInset(float value) {
            LayoutInsets newValue = insets;
            newValue.top = value;
            SetInsets(newValue);
        }

        void SetBottomInset(float value) {
            LayoutInsets newValue = insets;
            newValue.bottom = value;
            SetInsets(newValue);
        }

        // MARK: SomeViewLayout:

        Vector2 ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) override;
        void LayoutSubviews(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;

        String TypeName() const override {
            return "PadViewLayout";
        }
    };
} // namespace PJ
