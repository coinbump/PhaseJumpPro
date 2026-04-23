#pragma once

#include "MediaClipModel.h"
#include "MediaTimelineModel.h"
#include "View2D.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    class MediaTrackView;

    /// A DAW/NLE-style multitrack timeline view. Contains a vertical stack of MediaTrackView
    /// children and a shared MediaTimelineModel (duration + viewable window).
    class MediaTimelineView : public View2D {
    public:
        using Base = View2D;
        using This = MediaTimelineView;

        /// Called when MediaTimelineView needs to build the view content for a clip.
        /// Implementors should add renderers / child nodes that visualize the clip.
        struct BuildClipViewArgs {
            This& timeline;
            MediaTrackView& track;
            int trackIndex = 0;
            SP<MediaClipModel> clip;
            WorldNode& clipNode;
        };

        using BuildClipViewFunc = std::function<void(BuildClipViewArgs const&)>;

    protected:
        MediaTimelineModel timelineModel;

    public:
        /// Customization hook for building a clip's view. Called whenever a clip is added or
        /// created via a split. Defaults to a simple color block.
        BuildClipViewFunc buildClipViewFunc;

        MediaTimelineView();

        MediaTimelineModel& TimelineModel() {
            return timelineModel;
        }

        MediaTimelineModel const& TimelineModel() const {
            return timelineModel;
        }

        /// Adds a new empty track to the timeline.
        /// @return The newly added track view.
        MediaTrackView& AddTrack();

        /// @return Returns all track views, in stack order (top to bottom).
        VectorList<MediaTrackView*> TrackViews() const;

        /// Finds the track view closest to the given world Y position.
        /// @return nullptr if the timeline has no tracks.
        MediaTrackView* ClosestTrackView(Vector2 worldPosition) const;

        /// Inserts a new clip on the track closest to the given world position.
        /// Splits / removes overlapping existing clips on that track.
        void AddClip(Vector2 worldPosition, SP<MediaClipModel> clip);

        /// Builds (or rebuilds) the view for a clip on the given track, using
        /// `buildClipViewFunc` (if set) to populate the clip's content.
        WorldNode* BuildClipView(MediaTrackView& track, SP<MediaClipModel> clip);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "MediaTimelineView";
        }
    };
} // namespace PJ
