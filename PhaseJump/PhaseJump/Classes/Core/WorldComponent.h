#pragma once

#include "SomePosition.h"
#include "SomeWorldComponent.h"
#include "WorldComponentCores.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// World component with a core
    /// The core allows us to create components using composition instead of inheritance
    /// Alternatively, the core can be used to store additional properties and state
    template <class Core = VoidWorldComponentCore>
    class WorldComponent : public SomeWorldComponent {
    public:
        using Base = SomeWorldComponent;

        /// Allows us to build component functionality with composition instead of inheritance
        Core core{};

        WorldComponent(String name = "") :
            Base(name) {}

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return core.TypeName();
        }

        void Awake() override {
            core.Awake(*this);
        }

        void Start() override {
            core.Start(*this);
        }

        void LateUpdate() override {
            core.LateUpdate(*this);
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            core.OnUpdate(*this, time);
        }

        // FUTURE: virtual void UpdateFromSerializedProperties(bool forceUpdate) {}
    };
} // namespace PJ

/// Adds world component support to an object with no-op methods
#define WORLD_COMPONENT_SUPPORT_VOID(Type)   \
    using Component = WorldComponent<Type>;  \
    void Awake(Component& component) {}      \
    void Start(Component& component) {}      \
    void LateUpdate(Component& component) {} \
    void OnUpdate(Component& component, TimeSlice time) {}
