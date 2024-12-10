#include "FlowStackViewLayout.h"
#include "SpacerView.h"
#include "UIPlanner.h"
#include "View2D.h"

using namespace std;
using namespace PJ;

FlowStackViewLayout::FlowStackViewLayout(Axis2D _axis, float _spacing, AlignFunc alignFunc) :
    axis(_axis),
    spacing(_spacing),
    alignFunc(alignFunc) {
    PlanUIFunc planUIFunc = [this](auto& component, String context, UIPlanner& planner) {
        planner.InputFloat(
            "Spacing", { [this]() { return spacing; }, [this](auto& value) { SetSpacing(value); } }
        );
        planner.InputBool(
            "Vertical", { [this]() { return axis == Axis2D::Y; },
                          [this](auto& value) { SetAxis(value ? Axis2D::Y : Axis2D::X); } }
        );

        planner.PickerList(
            "Alignment", _editAlignOptions,
            { [this]() { return _editAlignOption ? *_editAlignOption : -1; },
              [this](auto& value) {
                  _editAlignOption = value;
                  switch (value) {
                  case 0:
                      SetAlignFunc(AlignFuncs::left);
                      break;
                  case 1:
                      SetAlignFunc(AlignFuncs::center);
                      break;
                  case 2:
                      SetAlignFunc(AlignFuncs::right);
                      break;
                  default:
                      break;
                  }
              } }
        );
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

Vector2
FlowStackViewLayout::ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) {
    GUARDR(proposal.width && proposal.height, Vector2::max)

    CalculateFramesIfNeeded(Vector2{ *proposal.width, *proposal.height }, children);
    GUARDR(calculatedFrames.size() > 0, {})

    float orthogonal{};
    for (auto& frame : calculatedFrames) {
        orthogonal = std::max(orthogonal, frame.size.AxisValueOrthogonal(axis));
    }

    auto lastFrame = calculatedFrames[calculatedFrames.size() - 1];
    Vector2 result;
    result.AxisValue(axis) =
        isAxisUnbounded ? FloatMath::maxValue
                        : lastFrame.origin.AxisValue(axis) + lastFrame.size.AxisValue(axis);
    result.AxisValueOrthogonal(axis) = isOrthogonalUnbounded ? FloatMath::maxValue : orthogonal;
    return result;
}

FlowStackViewLayout::UnboundedInfo FlowStackViewLayout::CalculateUnboundedInfo(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    ViewSizeProposal proposal{ .width = boundsSize.x, .height = boundsSize.y };

    VectorList<ViewProxy> boundedChildren;
    VectorList<ViewProxy> unboundedChildren;
    for (auto& child : children) {
        if (child.IsAxisUnbounded(axis, proposal)) {
            unboundedChildren.push_back(child);
        } else {
            boundedChildren.push_back(child);
        }
    }

    float boundedChildrenLength{};

    for (auto& child : boundedChildren) {
        auto viewSize = child.ViewSize(proposal);
        boundedChildrenLength += viewSize.AxisValue(axis);
    }

    float unboundedAvailableLength =
        (boundsSize.AxisValue(axis) - boundedChildrenLength) - (children.size() - 1) * spacing;
    return { .availableLength = unboundedAvailableLength,
             .boundedChildren = boundedChildren,
             .unboundedChildren = unboundedChildren };
}

void FlowStackViewLayout::CalculateFrames(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    GUARD(owner);
    GUARD(children.size() > 0);
    GUARD(calculatedFrames.size() == 0)

    VectorList<OrthogonalLayoutDimension> dimensions;

    auto unboundedInfo = CalculateUnboundedInfo(boundsSize, children);
    auto unboundedAvailableLength = unboundedInfo.availableLength;
    float unboundedChildCount = (float)unboundedInfo.unboundedChildren.size();

    float pos = 0;
    float maxChildOrthogonal{};
    isOrthogonalUnbounded = false;

    // If all children are unbounded along the axis, this view is also unbounded along the axis
    isAxisUnbounded = unboundedChildCount == children.size();

    for (auto& child : children) {
        float _length{};

        bool isLengthUnbounded{};
        if (isAxisUnbounded || ContainsIf(unboundedInfo.unboundedChildren, [&](auto& proxy) {
                return proxy.view == child.view;
            })) {
            isLengthUnbounded = true;

            // Unbounded
            _length = unboundedAvailableLength / unboundedChildCount;

            // Because of constraints, re-evaluate size for each unbounded child
            unboundedChildCount--;
        } else {
            ViewSizeProposal proposal{ .width = boundsSize.x, .height = boundsSize.y };
            Vector2 childSize = child.ViewSizeWithoutConstraints(proposal);
            _length = childSize.AxisValue(axis);
        }

        ViewSizeProposal childSizeProposal;
        childSizeProposal.AxisValue(axis) = _length;
        childSizeProposal.AxisValueOrthogonal(axis) = boundsSize.AxisValueOrthogonal(axis);

        // Derive orthogonal size
        Vector2 childSize = child.ViewSizeWithoutConstraints(childSizeProposal);

        float _orthogonal{};

        if (childSize.AxisValueOrthogonal(axis) == FloatMath::maxValue) {
            // Unbounded
            _orthogonal = boundsSize.AxisValueOrthogonal(axis);
        } else {
            _orthogonal = childSize.AxisValueOrthogonal(axis);
        }

        Vector2 viewSize;
        viewSize.AxisValue(axis) = _length;
        viewSize.AxisValueOrthogonal(axis) = _orthogonal;

        viewSize = child.view->WithSizeConstraints(viewSize);

        // Update available space after applying constraints
        if (isLengthUnbounded) {
            unboundedAvailableLength -= viewSize.AxisValue(axis);
        }

        OrthogonalLayoutDimension dimension = OrthogonalLayoutDimension::View;
        if (childSize.AxisValueOrthogonal(axis) != FloatMath::maxValue) {
            maxChildOrthogonal = std::max(maxChildOrthogonal, childSize.AxisValueOrthogonal(axis));
        } else {
            // Special case: Spacers do not expand the orthogonal direction of a stack view
            if (!Is<SpacerView>(child.view)) {
                isOrthogonalUnbounded = true;
                dimension = OrthogonalLayoutDimension::Bounds;
            } else {
                dimension = OrthogonalLayoutDimension::MaxChild;
            }
        }

        // Calculate what we can now, save the rest for later
        Vector2 origin;
        origin.AxisValue(axis) = pos;
        Rect frame{ .origin = origin, .size = viewSize };
        calculatedFrames.push_back(frame);
        dimensions.push_back(dimension);

        pos += (viewSize.AxisValue(axis) + spacing);
    }

    float boundsOrthogonalLength =
        isOrthogonalUnbounded ? boundsSize.AxisValueOrthogonal(axis) : maxChildOrthogonal;

    // Final alignment
    int index{};
    for (auto& frame : calculatedFrames) {
        auto dimension = dimensions[index];

        switch (dimension) {
        case OrthogonalLayoutDimension::View:
            break;
        case OrthogonalLayoutDimension::MaxChild:
            frame.size.AxisValueOrthogonal(axis) = maxChildOrthogonal;
            break;
        case OrthogonalLayoutDimension::Bounds:
            frame.size.AxisValueOrthogonal(axis) = boundsSize.AxisValueOrthogonal(axis);
            break;
        }

        if (alignFunc) {
            frame.origin.AxisValueOrthogonal(axis) =
                alignFunc(boundsOrthogonalLength, frame.size.AxisValueOrthogonal(axis));
        }

        index++;
    }
}

void FlowStackViewLayout::SetSpacing(float value) {
    GUARD(value != spacing);
    spacing = value;
    SetNeedsLayout();
}

void FlowStackViewLayout::SetAlignFunc(AlignFunc value) {
    alignFunc = value;
    SetNeedsLayout();
}

void FlowStackViewLayout::SetAxis(Axis2D value) {
    GUARD(axis != value)
    axis = value;
    SetNeedsLayout();
}
