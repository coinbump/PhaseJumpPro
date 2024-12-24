#include "SomeViewLayout.h"
#include "View2D.h"

using namespace std;
using namespace PJ;

void SomeViewLayout::SetNeedsLayout() {
    GUARD(owner)
    owner->SetNeedsLayout();
}

Vector2 SomeViewLayout::ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) {
    GUARDR(owner, {})

    // OPTIMIZE: early exit if this view has fixed size, no need to calculate it
    auto fixedWidth = owner->FixedWidth();
    auto fixedHeight = owner->FixedHeight();
    if (fixedWidth && fixedHeight) {
        return { *fixedWidth, *fixedHeight };
    }

    auto result = ViewSizeWithoutConstraints(proposal, children);
    result = owner->WithSizeConstraints(result);

    return result;
}

void ViewLayout::CalculateFramesIfNeeded(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    GUARD(calculatedSize != boundsSize);

    calculatedSize = boundsSize;
    calculatedFrames.clear();
    CalculateFrames(boundsSize, children);
}

void ViewLayout::LayoutSubviews(Vector2 boundsSize, VectorList<ViewProxy> const& children) {
    CalculateFramesIfNeeded(boundsSize, children);

    int index = 0;
    for (auto& child : children) {
        GUARD(index < calculatedFrames.size());

        auto frame = calculatedFrames[index];
        index++;

        // The layout determines view size, not the child
        ViewProxy proxy = child;
        proxy.viewSizeFunc = [=](auto& proxy, auto proposal) { return frame.size; };
        proxy.PlaceView(
            frame.origin, LayoutAnchor2D::topLeft, { .width = frame.size.x, .height = frame.size.y }
        );
    }
    return;
}

void ViewLayout::CalculateFramesIfNeeded(
    ViewSizeProposal proposal, VectorList<ViewProxy> const& children
) {
    float width = proposal.width ? *proposal.width : 0;
    float height = proposal.height ? *proposal.height : 0;

    CalculateFramesIfNeeded(Vector2{ width, height }, children);
}
