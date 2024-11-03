#pragma once

#include "SomePosition.h"
#include "SomeWorldComponent.h"
#include "StandardCore.h"
#include "Void.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// World component with a core
    template <class Core = StandardCore>
    class WorldComponent : public SomeWorldComponent {
    public:
        using Base = SomeWorldComponent;

        /// Allows us to build component functionality with composition instead of inheritance
        Core core{};

        WorldComponent(String name = "") :
            Base(name) {}

        // FUTURE: virtual void UpdateFromSerializedProperties(bool forceUpdate) {}

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "WorldComponent";
        }
    };
} // namespace PJ
