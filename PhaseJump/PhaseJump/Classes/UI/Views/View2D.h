#pragma once

#include "PointerClickUIEvent.h"
#include "Rect.h"
#include "UISystem.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"
#include <functional>
#include <optional>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: add ResolvedView methods
    class View2D : public WorldComponent<>, public WorldSizeable {
    public:
        using OptionalFloat = std::optional<float>;
        using IntrinsicLengthFunc = std::function<OptionalFloat(View2D&)>;
        using ProposedLengthFunc = std::function<OptionalFloat(View2D&, Vector2)>;

    protected:
        // Views can have independent intrinsic sizes
        // Example: intrinsicWidth is null, but intrinsicHeight is 10
        IntrinsicLengthFunc intrinsicWidthFunc;
        IntrinsicLengthFunc intrinsicHeightFunc;

        Rect frame;

        bool needsLayout = true;

    public:
        virtual OptionalFloat IntrinsicWidth() {
            GUARDR(intrinsicWidthFunc, {})
            return intrinsicWidthFunc(*this);
        }

        virtual OptionalFloat IntrinsicHeight() {
            GUARDR(intrinsicHeightFunc, {})
            return intrinsicHeightFunc(*this);
        }

        // MARK: - Bounds

        Rect Frame() const {
            return frame;
        }

        View2D& SetFrame(Rect const& value);

        /// Local bounds (origin is always zero)
        Rect Bounds() const;

        Vector2 WorldSize2D() const {
            return Frame().size;
        }

        void SetWorldSize2D(Vector2 value);

        // MARK: WorldSizeable

        std::optional<Vector3> WorldSize() const override {
            return frame.size;
        }

        void SetWorldSize(Vector3 value) override {
            SetFrame({ frame.origin, { value.x, value.y } });
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "View2D";
        }

    protected:
        virtual void OnFrameChange();
        virtual void UpdateFrameComponents();

    public:
        virtual std::optional<Rect> ParentBounds();

        // MARK: - Layout

        SP<View2D> ParentView() const;
        SP<View2D> FirstChildView() const;

        /// Called when the view size changes
        virtual void OnViewSizeChange();

        VectorList<SP<View2D>> ChildViews() const;

        ProposedLengthFunc proposeWidthWithoutConstraintsFunc;
        ProposedLengthFunc proposeWidthWithConstraintsFunc;
        ProposedLengthFunc proposeHeightWithoutConstraintsFuncs;
        ProposedLengthFunc proposeHeightWithConstraintsFunc;

        virtual OptionalFloat ProposedWidthWithoutConstraints(Vector2 layoutSize) {
            GUARDR(proposeWidthWithoutConstraintsFunc, {})
            return proposeWidthWithoutConstraintsFunc(*this, layoutSize);
        }

        virtual OptionalFloat ProposedWidthWithConstraints(Vector2 layoutSize) {
            GUARDR(proposeWidthWithConstraintsFunc, {})
            return proposeWidthWithConstraintsFunc(*this, layoutSize);
        }

        virtual OptionalFloat ProposedHeightWithoutConstraints(Vector2 layoutSize) {
            GUARDR(proposeHeightWithoutConstraintsFuncs, {})
            return proposeHeightWithoutConstraintsFuncs(*this, layoutSize);
        }

        virtual OptionalFloat ProposedHeightWithConstraints(Vector2 layoutSize) {
            GUARDR(proposeHeightWithConstraintsFunc, {})
            return proposeHeightWithConstraintsFunc(*this, layoutSize);
        }

    protected:
        virtual void _ApplyLayout(Vector2 layoutSize) {}

        // MARK: SomeComponent

        void Awake() override;
    };
} // namespace PJ
