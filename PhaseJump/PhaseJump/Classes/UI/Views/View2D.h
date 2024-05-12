#ifndef PJVIEW2D_H
#define PJVIEW2D_H

#include "Node2D.h"
#include "PointerClickUIEvent.h"
#include "UISystem.h"
#include "UISystemSharedDependencyResolver.h"
#include "Rect.h"
#include <optional>

namespace PJ {
    class View2D : public Node2D, public SomePointerEventsResponder {
    protected:
        std::optional<float> intrinsicWidth;
        std::optional<float> intrinsicHeight;

        Rect frame;

    public:
        virtual std::optional<float> IntrinsicHeight() { return std::nullopt; }

        SP<SomeDependencyResolver<SP<UISystem>>> uiSystemDependencyResolver = MAKE<UISystemSharedDependencyResolver>();

        virtual SP<UISystem> UISystem() const
        {
            return uiSystemDependencyResolver->Dependency();
        }

        // **************
        // MARK: - Bounds

        Rect Frame() const { return frame; }
        void SetFrame(Rect const& value);

        /// <summary>
        /// Local bounds (origin is always zero)
        /// </summary>
        Rect Bounds() const;

        Vector2 WorldSize2D() const { return Frame().size; }
        void SetWorldSize2D(Vector2 value);

    protected:
        virtual void OnFrameChange();
        virtual void UpdateFrameComponents();

    public:
        virtual Rect ParentBounds();

        // ***************
        // MARK: - Layout

        // TODO:
        SP<View2D> ParentView() const { return nullptr; }
        SP<View2D> FirstChildView() const { return nullptr; }
        virtual void OnViewSizeChange() {}
        virtual VectorList<SP<View2D>> ChildViews() const { return VectorList<SP<View2D>>(); }
        virtual std::optional<float> ProposedWidthWithoutConstraints(Vector2 layoutSize) { return std::nullopt; }
        virtual std::optional<float> ProposedWidthWithConstraints(Vector2 layoutSize) { return std::nullopt; }
        virtual std::optional<float> ProposedHeightWithoutConstraints(Vector2 layoutSize) { return std::nullopt; }
        virtual std::optional<float> ProposedHeightWithConstraints(Vector2 layoutSize) { return std::nullopt; }

    protected:
        virtual void _ApplyLayout(Vector2 layoutSize) {}
    };
}

#endif
