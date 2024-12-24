#pragma once

#include "ObservedValue.h"
#include "Rect.h"
#include "SomeViewLayout.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"
#include <functional>
#include <optional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/17/24
 */
namespace PJ {
    class ViewBuilder;

    /// Identifiers of common UI constraints
    namespace UIConstraintId {
        // Type: float
        auto constexpr FixedWidth = "fixed.width";
        auto constexpr FixedHeight = "fixed.height";
        auto constexpr MaxWidth = "max.width";
        auto constexpr MaxHeight = "max.height";
        auto constexpr PositionX = "pos.x";
        auto constexpr PositionY = "pos.y";
        auto constexpr OffsetX = "offset.x";
        auto constexpr OffsetY = "offset.y";

        // Type: bool
        auto constexpr IsIdealWidth = "isIntrinsic.width";
        auto constexpr IsIdealHeight = "isIntrinsic.height";
    } // namespace UIConstraintId

    /// Config for presenting a top level view (popovers, alerts)
    struct PresentTopLevelViewConfig {
        using MakeFrameFunc = std::function<Rect(View2D& presenterView, View2D& view)>;
        using BuildViewFunc = std::function<void(ViewBuilder&)>;

        /// Returns the frame for the top level view
        MakeFrameFunc makeFrameFunc;

        BuildViewFunc buildViewFunc;
    };

    /// Handles logic and layout for a reading-layout oriented node
    /// Example: windows, controls, buttons
    class View2D : public WorldComponent<>, public WorldSizeable {
    public:
        using Base = WorldComponent<>;
        using This = View2D;

        using IdealSizeFunc = std::function<ViewSizeProposal(This&, ViewSizeProposal)>;
        using BuildViewFunc = std::function<void(View2D&)>;

        /// If true, this view is an attachment to another view
        bool isAttachment{};

    protected:
        Rect frame;

        /// If true, the view will layout its subviews on demand
        bool needsLayout = true;

        bool needsRebuild{};

        /// Returns the intrinsic size of the view
        IdealSizeFunc idealSizeFunc;

        /// Layout for the view
        UP<SomeViewLayout> layout;

        /// @return Returns a list of view proxies of subviews for layout
        VectorList<ViewProxy> ChildViewProxies() const;

        /// Stores UI constraints, identifiers are defined in UIConstraintId
        Tags constraints;

        /// Environment variables (found by walking from child to root)
        Tags environment;

        // If true, this view is in hover state
        ObservedValue<bool> isHovering{ false };

        // If true, this is a top level view (popover, alert, etc.)
        bool isTopLevel{};

    public:
        /// Used to rebuild this view if the content changes
        BuildViewFunc buildViewFunc;

        View2D();

        void SetNeedsRebuild();
        void RebuildIfNeeded();
        virtual void Rebuild();

        template <class Type>
        Type EnvironmentValue(String id, Type defaultValue = {}) const {
            try {
                auto value = environment.At<Type>(id);
                return value;
            } catch (...) {
                auto parentView = ParentView();
                if (parentView) {
                    return parentView->EnvironmentValue<Type>(id);
                }
            }

            return defaultValue;
        }

        template <class Type>
        void SetEnvironmentValue(String id, Type value) {
            environment.Set(id, value);

            // Don't rebuild here. We might be in the render loop when editing environment values
            // from imGui
            SetNeedsRebuild();
        }

        SomeViewLayout* Layout() const {
            return layout.get();
        }

        // MARK: Hover

        bool IsHovering() const {
            return isHovering;
        }

        virtual void SetIsHovering(bool value) {
            isHovering = value;
        }

        /// @return Returns the intrinsic size (if any) for the view
        virtual ViewSizeProposal IdealSize(ViewSizeProposal proposal) {
            GUARDR(idealSizeFunc, {})
            return idealSizeFunc(*this, proposal);
        }

        void SetIdealSizeFunc(IdealSizeFunc value) {
            idealSizeFunc = value;
            SetNeedsLayout();
        }

        Rect WithFrameConstraints(Rect frame) const;
        Vector2 WithPositionConstraints(Vector2 origin) const;
        Vector2 WithSizeConstraints(Vector2 size) const;

        void SetOffset(std::optional<float> x, std::optional<float> y) {
            constraints.SetOrRemove(UIConstraintId::OffsetX, x);
            constraints.SetOrRemove(UIConstraintId::OffsetY, y);

            SetNeedsLayout();
        }

        void SetPosition(std::optional<float> x, std::optional<float> y) {
            constraints.SetOrRemove(UIConstraintId::PositionX, x);
            constraints.SetOrRemove(UIConstraintId::PositionY, y);

            SetNeedsLayout();
        }

        bool IsIdealWidth() const {
            return constraints.SafeValue<bool>(UIConstraintId::IsIdealWidth);
        }

        bool IsIdealHeight() const {
            return constraints.SafeValue<bool>(UIConstraintId::IsIdealHeight);
        }

        void SetIsIdealSize(bool isX, bool isY) {
            constraints.Set(UIConstraintId::IsIdealWidth, isX);
            constraints.Set(UIConstraintId::IsIdealHeight, isY);

            SetNeedsLayout();
        }

        std::optional<float> FixedWidth() const {
            return constraints.Value<float>(UIConstraintId::FixedWidth);
        }

        std::optional<float> FixedHeight() const {
            return constraints.Value<float>(UIConstraintId::FixedHeight);
        }

        void SetFixedSize(std::optional<float> x, std::optional<float> y) {
            constraints.SetOrRemove(UIConstraintId::FixedWidth, x);
            constraints.SetOrRemove(UIConstraintId::FixedHeight, y);

            SetNeedsLayout();
        }

        std::optional<float> MaxWidth() const {
            return constraints.Value<float>(UIConstraintId::MaxWidth);
        }

        std::optional<float> MaxHeight() const {
            return constraints.Value<float>(UIConstraintId::MaxHeight);
        }

        void SetMaxSize(std::optional<float> x, std::optional<float> y) {
            constraints.SetOrRemove(UIConstraintId::MaxWidth, x);
            constraints.SetOrRemove(UIConstraintId::MaxHeight, y);

            SetNeedsLayout();
        }

        /// @return Returns true if the view needs layout (only valid for root view)
        bool NeedsLayout() const {
            return RootView()->needsLayout;
        }

        /// Sets a flag to force the view to rebuild its layout
        void SetNeedsLayout();

        /// Rebuilds the view layout if needed
        void LayoutIfNeeded();

        /// Applies the layout manually
        /// Preferred: use `LayoutIfNeeded` instead
        void ApplyLayout();

        /// Sets the layout and applies it
        void SetLayout(UP<SomeViewLayout>& value);

        /// Used internally for view layouts
        virtual ViewProxy MakeViewProxy();

        // MARK: - Bounds

        Rect Frame() const {
            return frame;
        }

        View2D& SetFrame(Rect const& value);

        void SetFrameSize(Vector2 value) {
            SetFrame({ frame.origin, { value.x, value.y } });
        }

        /// @return Returns local bounds (origin is zero)
        Rect Bounds() const;

        /// @return Returns the 2D size of this view
        Vector2 WorldSize2D() const {
            return Frame().size;
        }

        /// Sets the 2D size of this view (equivalent to SetFrameSize)
        void SetWorldSize2D(Vector2 value);

        // MARK: Top level views

        bool IsTopLevel() const {
            return isTopLevel;
        }

        /// @return Returns a list of top level views that are currently presented
        VectorList<View2D*> TopLevelViews();

        /// Presents a top level view
        void Present(PresentTopLevelViewConfig config);

        /// Dismisses a top level view
        void Dismiss(View2D& view);

        /// Pops top level views until this view is the frontmost top level view
        void PopTo(View2D& view);

        /// Dismisses the topmost top level view
        void PopTopLevel();

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return frame.size;
        }

        void SetWorldSize(Vector3 value) override {
            SetFrame({ frame.origin, { value.x, value.y } });
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "View2D";
        }

        // MARK: Internal

        /// Called before layout is applied (internal use only)
        void StartLayout();

        /// @return Returns the bounds of the parent view, if any
        virtual std::optional<Rect> ParentBounds();

        /// @return Returns the parent view, if any
        View2D* ParentView() const;

        /// @return Returns the root view
        View2D* RootView() const;

        /// @return Returns true if this view is the root view
        bool IsRoot() const {
            return nullptr == ParentView();
        }

        /// @return Returns the first subview
        View2D* FirstChildView() const;

        /// @return Returns all subviews
        VectorList<View2D*> ChildViews() const;

        /// @return Returns the top-left of this view in world coordinates
        Vector3 TopLeftWorldPosition() const;

        /// @return returns true if the view position is inside this view's frame
        /// View position is in local reading coordinates
        bool TestViewPositionHit(Vector2 viewPosition) const;

        Vector2 ToViewPosition(LayoutAnchor2D viewPoint) const;
        Vector3 ViewToWorldPosition(Vector2 viewPosition) const;
        Vector3 ViewToLocalPosition(Vector2 viewPosition) const;
        Vector2 WorldToViewPosition(Vector3 worldPosition) const;

        /// @return Returns a view position, converted from the local position
        Vector2 LocalToViewPosition(Vector3 localPosition) const;

        /// @return Returns the top-left local position in this view's node for child views
        Vector3 TopLeftLocalPosition() const;

        /// @return Returns top left local position in this view's node for the child view
        Vector3 ChildTopLeftLocalPosition(Rect childFrame) const;

        /// @return Returns local position in this view's node to place the child view
        Vector3 ChildLocalPosition(Rect childFrame) const;

        /// @return Returns a position in this view converted to a position in the root view
        Vector2 ToRootViewPosition(Vector2 viewPosition);

    protected:
        /// Called when the frame changes
        virtual void OnFrameChange();

        /// Resizes sibling components to match the size of the view
        virtual void UpdateFrameComponents();

        /// Called when the view size changes
        virtual void OnViewSizeChange();

        // MARK: SomeComponent

        void Start() override;
    };
} // namespace PJ
