#include "MediaTimelineView.h"
#include "ColorRenderer.h"
#include "FlowStackViewLayout.h"
#include "MediaTrackView.h"
#include "WorldNode.h"
#include <limits>

using namespace std;
using namespace PJ;

namespace {
    constexpr float defaultTrackHeight = 100.0f;
}

MediaTimelineView::MediaTimelineView() {
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y, 0);
    SetLayout(layout);

    // Default: build a simple color block for each clip. Scenes override by setting
    // buildClipViewFunc
    buildClipViewFunc = [](BuildClipViewArgs const& args) {
        args.clipNode.AddComponent<ColorRenderer>(ColorRenderer::Config{ .color = Color::gray });
    };
}

MediaTrackView& MediaTimelineView::AddTrack() {
    GUARD_THROW(owner, std::runtime_error("MediaTimelineView has no owner node"))

    auto& trackNode = owner->And(WorldNode::Config{ .name = "MediaTrack" });
    auto& trackView = trackNode.AddComponent<MediaTrackView>();
    trackView.SetFixedSize({}, defaultTrackHeight);
    return trackView;
}

VectorList<MediaTrackView*> MediaTimelineView::TrackViews() const {
    VectorList<MediaTrackView*> result;
    GUARDR(owner, result)

    for (auto& child : owner->Children()) {
        auto trackView = child->TypeComponent<MediaTrackView>();
        if (trackView) {
            result.push_back(trackView);
        }
    }
    return result;
}

MediaTrackView* MediaTimelineView::ClosestTrackView(Vector2 worldPosition) const {
    auto tracks = TrackViews();
    GUARDR(!tracks.empty(), nullptr)

    // Convert world position into the timeline's view space for a straight Y comparison.
    auto viewPos =
        const_cast<MediaTimelineView*>(this)->WorldToView({ worldPosition.x, worldPosition.y, 0 });

    MediaTrackView* closest = nullptr;
    float bestDistance = std::numeric_limits<float>::max();

    for (auto track : tracks) {
        auto frame = track->Frame();
        float centerY = frame.origin.y + frame.size.y / 2.0f;
        float distance = std::fabs(viewPos.y - centerY);
        if (distance < bestDistance) {
            bestDistance = distance;
            closest = track;
        }
    }

    return closest;
}

WorldNode* MediaTimelineView::BuildClipView(MediaTrackView& track, SP<MediaClipModel> clip) {
    GUARDR(clip && track.owner, nullptr)

    auto& clipNode = track.owner->And(WorldNode::Config{ .name = "MediaClip" });
    clipNode.AddComponent<View2D>();

    track.RegisterClipView(clip, &clipNode);

    if (buildClipViewFunc) {
        auto tracks = TrackViews();
        int index = 0;
        for (size_t i = 0; i < tracks.size(); ++i) {
            if (tracks[i] == &track) {
                index = static_cast<int>(i);
                break;
            }
        }
        buildClipViewFunc({
            .timeline = *this,
            .track = track,
            .trackIndex = index,
            .clip = clip,
            .clipNode = clipNode,
        });
    }

    return &clipNode;
}

void MediaTimelineView::AddClip(Vector2 worldPosition, SP<MediaClipModel> clip) {
    GUARD(clip)

    auto track = ClosestTrackView(worldPosition);
    GUARD(track)

    // Map the drop X into timeline-time based on the track's view-space width and the
    // timeline's current window.
    auto trackViewPos = track->WorldToView({ worldPosition.x, worldPosition.y, 0 });
    auto window = timelineModel.Window();
    float const trackWidth = track->Frame().size.x;
    if (trackWidth > 0 && window.duration > 0) {
        float const normalizedX = std::clamp(trackViewPos.x / trackWidth, 0.0f, 1.0f);
        clip->start = window.start + normalizedX * window.duration;
    }

    auto result = track->TrackModel().Insert(clip);

    // Remove views for clips that were completely overwritten.
    for (auto& removed : result.removedClips) {
        track->UnregisterClipView(*removed);
    }

    // Rebuild views for clips whose model data changed (start / duration / sourceStart).
    // Destroy the previous node so renderers pick up the new time range cleanly.
    for (auto& modified : result.modifiedClips) {
        track->UnregisterClipView(*modified);
        BuildClipView(*track, modified);
    }

    // Create views for any new right-half clips from splits.
    for (auto& splitClip : result.splitClips) {
        BuildClipView(*track, splitClip);
    }

    // Create the view for the newly inserted clip.
    BuildClipView(*track, result.insertedClip);

    track->SetNeedsLayout();
}
