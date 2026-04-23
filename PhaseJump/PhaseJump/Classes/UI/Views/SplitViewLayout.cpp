#include "SplitViewLayout.h"
#include "Dev.h"
#include "FloatMath.h"
#include "View2D.h"

using namespace std;
using namespace PJ;

SplitViewLayout::SplitViewLayout(
    Axis2D axis, std::function<float(float)> firstViewSizeFunc,
    std::function<float(float)> middleViewSizeFunc
) :
    axis(axis) {
    if (firstViewSizeFunc) {
        this->firstViewSizeFunc = std::move(firstViewSizeFunc);
    }
    if (middleViewSizeFunc) {
        this->middleViewSizeFunc = std::move(middleViewSizeFunc);
    }

    // Fill available space
    viewSizeFunc = [](ViewLayout&, ViewSizeProposal, VectorList<ViewProxy> const&) {
        return Vector2{ FloatMath::maxValue, FloatMath::maxValue };
    };
}

void SplitViewLayout::CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) {
    calculatedFrames.clear();
    GUARD(!children.empty())

    if (children.size() > 3) {
        PJ::Log("WARNING: SplitViewLayout only supports up to 3 children.");
    }

    if (children.size() == 1) {
        calculatedFrames.push_back({ .origin = {}, .size = boundsSize });
        return;
    }

    float const axisLength = boundsSize.AxisValue(axis);
    float const orthogonalLength = boundsSize.AxisValueOrthogonal(axis);

    float firstLength = firstViewSizeFunc ? firstViewSizeFunc(axisLength) : axisLength / 2.0f;
    firstLength = std::clamp(firstLength, 0.0f, axisLength);

    float middleLength = 0.0f;
    if (children.size() >= 3) {
        middleLength = middleViewSizeFunc ? middleViewSizeFunc(axisLength) : 10.0f;
        middleLength = std::clamp(middleLength, 0.0f, axisLength - firstLength);
    }

    float const lastLength = std::max(0.0f, axisLength - firstLength - middleLength);

    auto makeFrame = [&](float offset, float length) {
        Rect frame{};
        frame.origin.AxisValue(axis) = offset;
        frame.size.AxisValue(axis) = length;
        frame.size.AxisValueOrthogonal(axis) = orthogonalLength;
        return frame;
    };

    calculatedFrames.push_back(makeFrame(0.0f, firstLength));

    if (children.size() == 2) {
        calculatedFrames.push_back(makeFrame(firstLength, lastLength));
    } else {
        // 3 or more children: first | middle (splitter) | last
        calculatedFrames.push_back(makeFrame(firstLength, middleLength));
        calculatedFrames.push_back(makeFrame(firstLength + middleLength, lastLength));
    }

    // Push empty frames for any extras so LayoutSubviews iterates safely.
    for (size_t i = calculatedFrames.size(); i < children.size(); ++i) {
        calculatedFrames.push_back({});
    }
}
