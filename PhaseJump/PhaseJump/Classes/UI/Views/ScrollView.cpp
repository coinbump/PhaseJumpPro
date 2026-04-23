#include "ScrollView.h"
#include "Dev.h"
#include "FloatMath.h"

using namespace std;
using namespace PJ;

ScrollView::Layout::Layout() {
    // ScrollView size is unbounded
    viewSizeFunc = [](ViewLayout&, ViewSizeProposal, VectorList<ViewProxy> const&) {
        return Vector2{ FloatMath::maxValue, FloatMath::maxValue };
    };
}

void ScrollView::Layout::CalculateFrames(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    calculatedFrames.clear();
    GUARD(!children.empty())

    auto scrollView = dynamic_cast<ScrollView*>(owner);
    if (children.size() > 1) {
        PJ::Log("WARNING: ScrollView only supports a single child view.");
    }

    Vector2 scrollOffset{};
    if (scrollView) {
        auto pos = scrollView->Position();
        if (scrollView->IsAxisEnabled(Axis2D::X)) {
            scrollOffset.x = -pos.x;
        }
        if (scrollView->IsAxisEnabled(Axis2D::Y)) {
            scrollOffset.y = -pos.y;
        }
    }

    for (size_t i = 0; i < children.size(); ++i) {
        auto const& child = children[i];

        ViewSizeProposal proposal{ .width = boundsSize.x, .height = boundsSize.y };
        Vector2 childSize = child.ViewSize(proposal);

        // Pin unbounded dimensions to the scroll view's bounds on axes that don't scroll.
        if (childSize.x == FloatMath::maxValue) {
            childSize.x = boundsSize.x;
        }
        if (childSize.y == FloatMath::maxValue) {
            childSize.y = boundsSize.y;
        }

        Rect frame{ .origin = (i == 0) ? scrollOffset : Vector2{}, .size = childSize };
        calculatedFrames.push_back(frame);
    }
}

ScrollView::ScrollView() {
    UP<SomeViewLayout> layout = NEW<ScrollView::Layout>();
    SetLayout(layout);

    position.SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [this](Vector2) {
        SetNeedsLayout();
    });
}
