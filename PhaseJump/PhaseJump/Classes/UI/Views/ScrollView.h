#pragma once

#include "Axis.h"
#include "ObservedValue.h"
#include "SomeViewLayout.h"
#include "UnorderedSet.h"
#include "Vector2.h"
#include "View2D.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// A view that scrolls its single child along the enabled axes.
    /// The scroll amount is the view-space offset applied (negated) to the child's position.
    class ScrollView : public View2D {
    public:
        using Base = View2D;
        using This = ScrollView;

        /// Layout that positions the scroll view's single child at -scrollPosition along the
        /// enabled axes.
        class Layout : public ViewLayout {
        public:
            using Base = ViewLayout;
            using This = Layout;

            Layout();

            String TypeName() const override {
                return "ScrollView::Layout";
            }

        protected:
            void
            CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;
        };

        /// Current scroll offset in view-space.
        ObservedValue<Vector2> position{ {} };

        /// Axes that respond to scroll position. Default: Y only.
        UnorderedSet<Axis2D> enabledAxes{ Axis2D::Y };

        ScrollView();

        Vector2 Position() const {
            return position.Value();
        }

        void SetPosition(Vector2 value) {
            position = value;
        }

        UnorderedSet<Axis2D> const& EnabledAxes() const {
            return enabledAxes;
        }

        void SetEnabledAxes(UnorderedSet<Axis2D> value) {
            enabledAxes = std::move(value);
            SetNeedsLayout();
        }

        bool IsAxisEnabled(Axis2D axis) const {
            return enabledAxes.find(axis) != enabledAxes.end();
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ScrollView";
        }
    };
} // namespace PJ
