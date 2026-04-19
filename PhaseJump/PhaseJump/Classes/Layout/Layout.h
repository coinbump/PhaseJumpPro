#pragma once

#include "Macros.h"
#include "Updatable.h"
#include "Vector3.h"
#include <functional>
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    class WorldComponent;

    class Layout {
    public:
        using This = Layout;
        using ApplyLayoutFunc = std::function<void(This&)>;

        /// Called to arrange the children. Subclass sets this to dispatch to its
        /// own layout implementation
        ApplyLayoutFunc applyLayoutFunc;

        /// If true, layout will be applied automatically
        /// If false, you must apply layout manually
        bool autoApply = true;

        /// Receives update events and triggers a layout pass when needed
        Updatable updatable;

    protected:
        bool needsLayout = true;

    public:
        /// Arrange child objects according to the layout
        void ApplyLayout() {
            GUARD(applyLayoutFunc)
            applyLayoutFunc(*this);
        }

        void LayoutIfNeeded() {
            GUARD(needsLayout);
            needsLayout = false;
            ApplyLayout();
        }

        bool NeedsLayout() const {
            return needsLayout;
        }

        void SetNeedsLayout() {
            needsLayout = true;
        }

        /// Wires up the enclosing component's update and signal dispatch
        void Awake(WorldComponent& component);

        /// Applies layout if needed when the component starts
        void Start(WorldComponent& component);
    };
} // namespace PJ
