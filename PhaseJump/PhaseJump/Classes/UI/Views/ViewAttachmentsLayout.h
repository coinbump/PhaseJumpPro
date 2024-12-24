#pragma once

#include "Macros.h"
#include "ZStackViewLayout.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 11/29/24
 */
namespace PJ {
    /// Matches the size of view attachments to the reference view
    /// Used for background and overlay views
    class ViewAttachmentsLayout : public ZStackViewLayout {
    public:
        using Base = ZStackViewLayout;
        using This = ViewAttachmentsLayout;

    protected:
        // MARK: ViewLayout

        void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;

        void StartLayout() override {
            ViewLayout::StartLayout();
        }

    public:
        ViewAttachmentsLayout() {}

        // MARK: SomeWorldComponent:

        String TypeName() const override {
            return "MatchSizeViewLayout";
        }
    };
} // namespace PJ
