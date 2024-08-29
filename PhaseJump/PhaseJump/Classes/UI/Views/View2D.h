#pragma once

#include "Node2D.h"
#include "PointerClickUIEvent.h"
#include "Rect.h"
#include "UISystem.h"
#include <optional>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: add ResolvedView methods
    class View2D : public Node2D, public SomePointerUIEventsResponder {

    protected:
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

        void SetFrame(Rect const& value);

        /// Local bounds (origin is always zero)
        Rect Bounds() const;

        Vector2 WorldSize2D() const {
            return Frame().size;
        }

        void SetWorldSize2D(Vector2 value);

    protected:
        virtual void OnFrameChange();
        virtual void UpdateFrameComponents();

    public:
        virtual Rect ParentBounds();

        // ***************
        // MARK: - Layout

        // TODO:
        SP<View2D> ParentView() const {
            return nullptr;
        }

        SP<View2D> FirstChildView() const {
            return nullptr;
        }

        virtual void OnViewSizeChange() {}

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
    };
} // namespace PJ
