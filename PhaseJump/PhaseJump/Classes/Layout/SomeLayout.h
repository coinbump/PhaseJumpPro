#pragma once

#include "Vector3.h"
#include "WorldComponent.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Defines the layout of child world nodes
    class SomeLayout : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = SomeLayout;

    protected:
        bool layoutNeedsBuild = true;

        /// Arrange child objects according to the layout
        virtual void ApplyLayout() = 0;

    public:
        /// If true, layout will be applied automatically
        /// If false, you must apply layout manually
        bool autoApply = true;

        void LayoutIfNeeded() {
            GUARD(layoutNeedsBuild);
            layoutNeedsBuild = false;
            ApplyLayout();
        }

        void SetNeedsLayout() {
            layoutNeedsBuild = true;
        }

    protected:
        void Awake() override;
    };
} // namespace PJ
