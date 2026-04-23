#pragma once

#include "Base.h"
#include "MediaClipModel.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// Model for a single media track containing an ordered list of clips.
    /// Insertions are "destructive": new clips split or remove overlapping existing clips,
    /// the same way clips are dropped onto a DAW/NLE track.
    class MediaTrackModel {
    public:
        using This = MediaTrackModel;

        /// Result of inserting a clip into the track.
        struct InsertResult {
            /// The clip that was inserted (stored in the track).
            SP<MediaClipModel> insertedClip;

            /// Clips created by splitting an existing clip around the new clip.
            VectorList<SP<MediaClipModel>> splitClips;

            /// Clips whose start or duration was modified to accommodate the new clip.
            VectorList<SP<MediaClipModel>> modifiedClips;

            /// Clips that were entirely removed because the new clip covers them.
            VectorList<SP<MediaClipModel>> removedClips;
        };

    protected:
        VectorList<SP<MediaClipModel>> clips;

    public:
        VectorList<SP<MediaClipModel>> const& Clips() const {
            return clips;
        }

        /// @return True if the track has no clips.
        bool IsEmpty() const {
            return clips.empty();
        }

        /// Inserts a new clip, splitting or removing overlapping existing clips as needed.
        /// The inserted clip is stored by shared pointer and returned in the result.
        InsertResult Insert(SP<MediaClipModel> clip);
    };
} // namespace PJ
