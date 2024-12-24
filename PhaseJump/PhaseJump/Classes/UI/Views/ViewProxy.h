#pragma once

#include "LayoutTypes.h"
#include "Vector2.h"
#include "ViewSizeProposal.h"
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/16/24
 */
namespace PJ {
    class View2D;

    /// Proxy representation of a view for layout
    /// Decouples layout from view types
    class ViewProxy {
    public:
        using This = ViewProxy;

        View2D* view{};

        ViewProxy(View2D* view) :
            view(view) {}

        using ViewSizeFunc = std::function<Vector2(This const&, ViewSizeProposal)>;
        using PlaceViewFunc = std::function<void(This&, Vector2, LayoutAnchor2D, ViewSizeProposal)>;

        ViewSizeFunc viewSizeFunc;
        PlaceViewFunc placeViewFunc;

        Vector2 ViewSize(ViewSizeProposal proposal) const;

        /// Return a size for the view based on the size proposal (without constraints)
        Vector2 ViewSizeWithoutConstraints(ViewSizeProposal proposal) const;

        /// @return Returns true if the view width has no limit
        bool IsWidthUnbounded(ViewSizeProposal proposal) const;

        /// @return Returns true if the view height has no limit
        bool IsHeightUnbounded(ViewSizeProposal proposal) const;

        /// @return Returns true if the view dimension along the specified axis has no limit
        bool IsAxisUnbounded(Axis2D axis, ViewSizeProposal proposal) const;

        /// Place the view at the specified anchor with the size proposal
        void PlaceView(Vector2 viewPosition, LayoutAnchor2D anchor, ViewSizeProposal proposal);

        /// @return Returns a view size func that specifies a fixed size
        static ViewSizeFunc MakeFixedViewSizeFunc(Vector2 size) {
            return [=](auto& proxy, auto proposal) { return size; };
        }

        static PlaceViewFunc MakePlaceViewFunc(View2D& view);
    };
} // namespace PJ
