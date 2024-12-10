#include "MatchSizeViewLayout.h"
#include "View2D.h"

using namespace std;
using namespace PJ;

Vector2
MatchSizeViewLayout::ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) {
    auto lock = matchView.lock();
    GUARDR(lock, {});

    CalculateFramesIfNeeded(proposal, children);

    GUARDR(matchSize, {});
    return *matchSize;
}

void MatchSizeViewLayout::CalculateFrames(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    auto lock = matchView.lock();

    Vector2 viewSize{};
    if (lock) {
        ViewProxy proxy = lock->MakeViewProxy();
        ViewSizeProposal proposal{ .width = boundsSize.x, .height = boundsSize.y };
        viewSize = proxy.ViewSize(proposal);
    }

    matchSize = viewSize;

    Rect frame{ .size = viewSize };

    // All child frames match the size of the match view
    Repeat(children.size(), [&]() { calculatedFrames.push_back(frame); });
}
