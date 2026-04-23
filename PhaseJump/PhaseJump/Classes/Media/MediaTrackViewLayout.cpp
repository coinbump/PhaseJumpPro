#include "MediaTrackViewLayout.h"
#include "FloatMath.h"
#include "MediaClipModel.h"
#include "MediaTimelineView.h"
#include "MediaTrackView.h"
#include "View2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

MediaTrackViewLayout::MediaTrackViewLayout() {
    // Track fills the parent timeline's width; the track view's fixedHeight constraint pins
    // its axis length. Without this, a parent stack layout would give the track width 0.
    viewSizeFunc = [](ViewLayout&, ViewSizeProposal, VectorList<ViewProxy> const&) {
        return Vector2{ FloatMath::maxValue, FloatMath::maxValue };
    };
}

void MediaTrackViewLayout::CalculateFrames(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    GUARD(owner)
    calculatedFrames.clear();

    auto trackView = dynamic_cast<MediaTrackView*>(owner);
    GUARD(trackView)

    // Walk up to find the timeline view and its viewable window.
    StreamWindow window;
    auto parentView = trackView->ParentView();
    while (parentView) {
        auto timelineView = dynamic_cast<MediaTimelineView*>(parentView);
        if (timelineView) {
            window = timelineView->TimelineModel().Window();
            break;
        }
        parentView = parentView->ParentView();
    }

    float const windowDuration = window.duration;
    float const windowStart = window.start;

    for (auto& child : children) {
        Rect frame{};
        frame.size.y = boundsSize.y;

        if (child.view && child.view->owner && windowDuration > 0) {
            auto entry = trackView->EntryForNode(child.view->owner);
            if (entry && entry->model) {
                float const normalStart = (entry->model->start - windowStart) / windowDuration;
                float const normalEnd = (entry->model->End() - windowStart) / windowDuration;
                frame.origin.x = normalStart * boundsSize.x;
                frame.size.x = (normalEnd - normalStart) * boundsSize.x;
            }
        }

        calculatedFrames.push_back(frame);
    }
}
