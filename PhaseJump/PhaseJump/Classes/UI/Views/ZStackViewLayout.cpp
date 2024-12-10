#include "ZStackViewLayout.h"
#include "FloatMath.h"

using namespace std;
using namespace PJ;

Vector2
ZStackViewLayout::ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) {
    GUARDR(proposal.width && proposal.height, Vector2::max)

    CalculateFramesIfNeeded(Vector2{ *proposal.width, *proposal.height }, children);
    GUARDR(calculatedFrames.size() > 0, {})

    Vector2 result;

    result = { isWidthUnbounded ? FloatMath::maxValue : maxChildWidth,
               isHeightUnbounded ? FloatMath::maxValue : maxChildHeight };
    return result;
}

void ZStackViewLayout::CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) {
    GUARD(owner);
    GUARD(children.size() > 0);
    GUARD(calculatedFrames.size() == 0)

    isWidthUnbounded = false;
    isHeightUnbounded = false;
    maxChildWidth = 0;
    maxChildHeight = 0;

    ViewSizeProposal proposal{ .width = boundsSize.x, .height = boundsSize.y };

    for (auto& child : children) {
        float _width{};

        auto childSize = child.ViewSizeWithoutConstraints(proposal);

        if (childSize.x == FloatMath::maxValue) {
            _width = boundsSize.x;
            isWidthUnbounded = true;
        } else {
            _width = childSize.x;
            maxChildWidth = std::max(maxChildWidth, childSize.x);
        }

        float _height{};

        if (childSize.y == FloatMath::maxValue) {
            _height = boundsSize.y;
            isHeightUnbounded = true;
        } else {
            _height = childSize.y;
            maxChildHeight = std::max(maxChildHeight, childSize.y);
        }

        Rect frame{ .origin = {}, .size = { _width, _height } };
        calculatedFrames.push_back(frame);
    }

    for (auto& frame : calculatedFrames) {
        if (xAlignFunc) {
            auto boundsSizeX = isWidthUnbounded ? boundsSize.x : maxChildWidth;
            frame.origin.x = xAlignFunc(boundsSizeX, frame.size.x);
        }
        if (yAlignFunc) {
            auto boundsSizeY = isHeightUnbounded ? boundsSize.y : maxChildHeight;
            frame.origin.y = yAlignFunc(boundsSizeY, frame.size.y);
        }
    }
}
