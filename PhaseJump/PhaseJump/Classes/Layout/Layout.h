#pragma once

#include "Vector3.h"
#include "WorldComponent.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    /// Defines the layout of child world nodes
    class Layout : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = Layout;

    protected:
        using LayoutFunc = std::function<void(This&)>;

        LayoutFunc applyLayoutFunc;

        bool needsLayout = true;

        /// Arrange child objects according to the layout
        virtual void ApplyLayout() {
            GUARD(applyLayoutFunc)
            applyLayoutFunc(*this);
        }

    public:
        /// If true, layout will be applied automatically
        /// If false, you must apply layout manually
        bool autoApply = true;

        void LayoutIfNeeded() {
            GUARD(needsLayout);
            needsLayout = false;
            ApplyLayout();
        }

        void SetNeedsLayout() {
            needsLayout = true;
        }

    protected:
        void Awake() override;
        void Start() override;
    };
} // namespace PJ
