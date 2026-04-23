#pragma once

#include "MediaClipModel.h"
#include "MediaTrackModel.h"
#include "View2D.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// A single horizontal track on a media timeline
    class MediaTrackView : public View2D {
    public:
        using Base = View2D;
        using This = MediaTrackView;

        /// Maps a clip model to the world node that renders it.
        struct ClipViewItem {
            SP<MediaClipModel> model;
            WorldNode* node{};
        };

    protected:
        MediaTrackModel trackModel;
        VectorList<ClipViewItem> clipViewItems;

    public:
        MediaTrackView();

        MediaTrackModel& TrackModel() {
            return trackModel;
        }

        MediaTrackModel const& TrackModel() const {
            return trackModel;
        }

        VectorList<ClipViewItem> const& ClipViewEntries() const {
            return clipViewItems;
        }

        /// Registers the world node that renders a clip model. Called by MediaTimelineView
        /// after it builds a clip view.
        void RegisterClipView(SP<MediaClipModel> model, WorldNode* node);

        /// Removes the registration (and the world node) for a clip model.
        void UnregisterClipView(MediaClipModel const& model);

        /// @return Returns the entry for the given node, or nullptr if not found.
        ClipViewItem const* EntryForNode(WorldNode const* node) const;

        /// @return Returns the entry for the given clip model, or nullptr if not found.
        ClipViewItem const* EntryForModel(MediaClipModel const& model) const;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "MediaTrackView";
        }
    };
} // namespace PJ
