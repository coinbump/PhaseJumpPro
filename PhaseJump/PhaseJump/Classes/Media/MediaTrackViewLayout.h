#pragma once

#include "SomeViewLayout.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// Layout for a MediaTrackView. Positions clip views horizontally based on their clip model
    /// start/duration, mapped through the timeline's viewable window (DAW/NLE style).
    class MediaTrackViewLayout : public ViewLayout {
    public:
        using Base = ViewLayout;
        using This = MediaTrackViewLayout;

        MediaTrackViewLayout();

        // MARK: SomeViewLayout

        String TypeName() const override {
            return "MediaTrackViewLayout";
        }

        // MARK: ViewLayout

    protected:
        void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;
    };
} // namespace PJ
