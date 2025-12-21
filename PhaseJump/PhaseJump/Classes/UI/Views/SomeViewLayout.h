#pragma once

#include "Rect.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include "VectorList.h"
#include "ViewProxy.h"
#include "ViewSizeProposal.h"
#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/16/24
 */
namespace PJ {
    class UIPlanner;

    enum class OrthogonalLayoutDimension {
        /// Use the calculated view orthogonal length
        View,

        /// Use the max child orthogonal length
        MaxChild,

        /// Use the layout bounds orthogonal length
        Bounds
    };

    /// View layout protocol
    class SomeViewLayout {
    public:
        using This = SomeViewLayout;

        struct PlanUIArgs {
            This& layout;
            String context;
            UIPlanner& planner;
        };

        using PlanUIFunc = std::function<void(PlanUIArgs args)>;

        virtual ~SomeViewLayout() {}

        /// Sets the layout owner view
        virtual void SetOwner(View2D* value) = 0;

        /// Forces the layout to update
        virtual void SetNeedsLayout() = 0;

        /// @return Returns a plan UI func, if any for UI planners
        virtual PlanUIFunc GetPlanUIFunc(String id) = 0;

        /// @return Returns the type name  for browsers and debugging
        virtual String TypeName() const = 0;

        /// Called before layout starts. Use to clear caches
        virtual void StartLayout() = 0;

        /// @return Returns the size of the view based on the view size proposal and children,
        /// including constraints
        virtual Vector2
        ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) = 0;

        /// @return Returns the size of the view based on the view size proposal and children
        virtual Vector2 ViewSizeWithoutConstraints(
            ViewSizeProposal proposal, VectorList<ViewProxy> const& children
        ) = 0;

        /// Places the subviews in the specified bounds
        virtual void LayoutSubviews(Vector2 boundsSize, VectorList<ViewProxy> const& children) = 0;
    };

    /// Layout for a view
    class ViewLayout : public SomeViewLayout {
    protected:
        /// Cached frames for the most recent view size proposal
        VectorList<Rect> calculatedFrames;

        /// View size that was used to calculate the frames
        std::optional<Vector2> calculatedSize;

        void
        CalculateFramesIfNeeded(ViewSizeProposal proposal, VectorList<ViewProxy> const& children);

        virtual void
        CalculateFramesIfNeeded(Vector2 boundsSize, VectorList<ViewProxy> const& children);

        virtual void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) {}

    public:
        using This = ViewLayout;

        View2D* owner{};

        /// Func to make UI plan for custom UI in editor
        UnorderedMap<String, PlanUIFunc> planUIFuncs;

        using ViewSizeFunc = std::function<
            Vector2(This&, ViewSizeProposal proposal, VectorList<ViewProxy> const& children)>;

        ViewSizeFunc viewSizeFunc;

        // MARK: SomeViewLayout

        PlanUIFunc GetPlanUIFunc(String id) override;

        void SetOwner(View2D* value) override {
            this->owner = value;
        }

        void SetNeedsLayout() override;

        Vector2 ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) override;

        void StartLayout() override {
            calculatedFrames.clear();
            calculatedSize = {};
        }

        /// @return Returns the size of the view based on the view size proposal and children
        Vector2 ViewSizeWithoutConstraints(
            ViewSizeProposal proposal, VectorList<ViewProxy> const& children
        ) override {
            GUARDR(viewSizeFunc, {})
            return viewSizeFunc(*this, proposal, children);
        }

        /// Places the subviews in the specified bounds
        virtual void
        LayoutSubviews(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;
    };
} // namespace PJ
