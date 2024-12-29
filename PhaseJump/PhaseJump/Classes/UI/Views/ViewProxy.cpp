#include "ViewProxy.h"
#include "View2D.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

ViewProxy::PlaceViewFunc ViewProxy::MakePlaceViewFunc(View2D& view) {
    return [&view](auto& proxy, auto viewPosition, auto anchor, auto proposal) {
        auto parentView = view.ParentView();
        GUARD(parentView)

        Vector2 viewSize = proxy.ViewSize(proposal);
        view.SetFrameSize(viewSize);

        auto anchorViewPosition = view.ToViewPosition(anchor);
        Vector2 topLeftPositionInParent = -anchorViewPosition + viewPosition;

        Vector3 topLeftLocalPosition = parentView->ViewToLocal(topLeftPositionInParent);

        Vector3 localPosition{ topLeftLocalPosition.x + (viewSize.x / 2.0f) * vecRight,
                               topLeftLocalPosition.y + (viewSize.y / 2.0f) * vecDown,
                               topLeftLocalPosition.z };

        view.SetFrame({ .origin = topLeftPositionInParent, .size = viewSize });
        view.owner->SetLocalPosition(localPosition);
    };
}

Vector2 ViewProxy::ViewSize(ViewSizeProposal proposal) const {
    auto result = ViewSizeWithoutConstraints(proposal);

    GUARDR(view, result)
    result = view->WithSizeConstraints(result);

    return result;
}

Vector2 ViewProxy::ViewSizeWithoutConstraints(ViewSizeProposal proposal) const {
    GUARDR(viewSizeFunc, {})
    return viewSizeFunc(*this, proposal);
}

bool ViewProxy::IsWidthUnbounded(ViewSizeProposal proposal) const {
    auto viewSize = ViewSizeWithoutConstraints(proposal);
    return viewSize.x == FloatMath::maxValue;
}

bool ViewProxy::IsHeightUnbounded(ViewSizeProposal proposal) const {
    auto viewSize = ViewSizeWithoutConstraints(proposal);
    return viewSize.y == FloatMath::maxValue;
}

bool ViewProxy::IsAxisUnbounded(Axis2D axis, ViewSizeProposal proposal) const {
    switch (axis) {
    case Axis2D::X:
        return IsWidthUnbounded(proposal);
        break;
    case Axis2D::Y:
        return IsHeightUnbounded(proposal);
        break;
    }
}

void ViewProxy::PlaceView(Vector2 viewPosition, LayoutAnchor2D anchor, ViewSizeProposal proposal) {
    GUARD(view)
    viewPosition = view->WithPositionConstraints(viewPosition);

    GUARD(placeViewFunc)
    placeViewFunc(*this, viewPosition, anchor, proposal);
}
