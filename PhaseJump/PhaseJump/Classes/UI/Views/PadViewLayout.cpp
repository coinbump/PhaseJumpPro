#include "PadViewLayout.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

PadViewLayout::PadViewLayout(LayoutInsets _insets) :
    insets(_insets) {
    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        planner.InputFloat({ .label = "Leading",
                             .binding = { [this]() { return insets.leading; },
                                          [this](auto& value) { SetLeadingInset(value); } } });
        planner.InputFloat({ .label = "Trailing",
                             .binding = { [this]() { return insets.trailing; },
                                          [this](auto& value) { SetTrailingInset(value); } } });
        planner.InputFloat({ .label = "Top",
                             .binding = { [this]() { return insets.top; },
                                          [this](auto& value) { SetTopInset(value); } } });
        planner.InputFloat({ .label = "Bottom",
                             .binding = { [this]() { return insets.bottom; },
                                          [this](auto& value) { SetBottomInset(value); } } });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

Vector2 PadViewLayout::ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) {
    GUARDR(proposal.width && proposal.height, Vector2::max)
    GUARDR(children.size() > 0, {});

    *proposal.width -= insets.HorizontalSum();
    *proposal.height -= insets.VerticalSum();

    auto child = children[0];
    auto childViewSize = child.ViewSize(proposal);

    bool isWidthUnbounded = childViewSize.x == FloatMath::maxValue;
    bool isHeightUnbounded = childViewSize.y == FloatMath::maxValue;

    Vector2 result{
        isWidthUnbounded ? FloatMath::maxValue : childViewSize.x + insets.HorizontalSum(),
        isHeightUnbounded ? FloatMath::maxValue : childViewSize.y + insets.VerticalSum()
    };
    return result;
}

void PadViewLayout::LayoutSubviews(Vector2 boundsSize, VectorList<ViewProxy> const& children) {
    GUARD(children.size() > 0);

    auto child = children[0];

    Vector2 viewSize{ boundsSize.x - insets.HorizontalSum(), boundsSize.y - insets.VerticalSum() };

    // The layout determines view size, not the child
    ViewProxy proxy = child;
    proxy.viewSizeFunc = [=](auto& proxy, auto proposal) { return viewSize; };
    proxy.PlaceView(
        { insets.leading, insets.top }, LayoutAnchor2D::topLeft, { viewSize.x, viewSize.y }
    );
}

void PadViewLayout::SetInsets(LayoutInsets value) {
    GUARD(value != insets)
    insets = value;
    SetNeedsLayout();
}
