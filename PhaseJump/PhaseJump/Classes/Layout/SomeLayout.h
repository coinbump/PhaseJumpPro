#pragma once

#include "Vector3.h"
#include "WorldComponent.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Defines the layout of child world nodes
    class SomeLayout : public WorldComponent<ComposeWorldComponentCore> {
    public:
        using Base = WorldComponent<ComposeWorldComponentCore>;
        using This = SomeLayout;

        /// If true, layout will be applied automatically
        /// If false, you must apply layout manually
        bool autoApply = true;

        /// Return the bounds-size of the layout
        virtual Vector3 Size() const = 0;

        /// Arrange child objects according to the layout
        virtual void ApplyLayout() = 0;

    protected:
        void Awake() override {
            Base::Awake();

            auto applyLayoutAction = [](SomeWorldComponent& worldComponent) {
                auto thisComponent = static_cast<This*>(&worldComponent);
                if (thisComponent->autoApply) {
                    thisComponent->ApplyLayout();
                }
            };

            core.startFuncs.Add(applyLayoutAction);
            core.lateUpdateFuncs.Add(applyLayoutAction);
        }
    };
} // namespace PJ
