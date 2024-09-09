#pragma once

#include "PointerClickUIEvent.h"
#include "Rect.h"
#include "UISystem.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"
#include <optional>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: add ResolvedView methods
    class View2D : public WorldComponent<>,
                   public SomePointerUIEventsResponder,
                   public WorldSizeable {

    protected:
        // ?? Isn't this the same thing as optional worldsize? NO. Because a view might have a value
        // for width, but not height
        std::optional<float> intrinsicWidth;
        std::optional<float> intrinsicHeight;

        Rect frame;

    public:
        virtual std::optional<float> IntrinsicHeight() {
            return std::nullopt;
        }

        std::function<UISystem*()> uiSystemResolver = []() { return UISystem::shared; };

        virtual UISystem* UISystem() const {
            GUARDR(uiSystemResolver, nullptr)
            return uiSystemResolver();
        }

        // **************
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

        // TODO: get rid of all WorldSize2D??
        void SetWorldSize2D(Vector2 value);

        // MARK: WorldSizeable

        std::optional<Vector3> WorldSize() const override {
            return frame.size;
        }

        void SetWorldSize(Vector3 value) override {
            SetFrame({ frame.origin, { value.x, value.y } });
        }

    protected:
        virtual void OnFrameChange();
        virtual void UpdateFrameComponents();

    public:
        virtual std::optional<Rect> ParentBounds();

        // ***************
        // MARK: - Layout

        // TODO:
        View2D* ParentView() const;
        SP<View2D> FirstChildView() const;

        virtual void OnViewSizeChange();

        virtual VectorList<SP<View2D>> ChildViews() const {
            return VectorList<SP<View2D>>();
        }

        virtual std::optional<float> ProposedWidthWithoutConstraints(Vector2 layoutSize) {
            return std::nullopt;
        }

        virtual std::optional<float> ProposedWidthWithConstraints(Vector2 layoutSize) {
            return std::nullopt;
        }

        virtual std::optional<float> ProposedHeightWithoutConstraints(Vector2 layoutSize) {
            return std::nullopt;
        }

        virtual std::optional<float> ProposedHeightWithConstraints(Vector2 layoutSize) {
            return std::nullopt;
        }

    protected:
        virtual void _ApplyLayout(Vector2 layoutSize) {}

        // MARK: SomeComponent

        void Awake() override;
    };
} // namespace PJ
