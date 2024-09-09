#pragma once

#include "SomePosition.h"
#include "SomeWorldComponent.h"
#include "Updatable.h"
#include "Updatables.h"
#include "WorldComponentCores.h"
#include "WorldNode.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Adds convenience methods that operate on WorldNode
    /// We can't declare these in SomeWorldComponent because it would create a
    /// circular include between SomeWorldComponent and WorldNode
    /// (template funcs can't be moved to the .cpp file to avoid this)
    template <class Core = VoidWorldComponentCore>
    class WorldComponent : public SomeWorldComponent {
    public:
        using Base = SomeWorldComponent;
        using ComponentList = WorldNode::ComponentList;
        using NodeList = WorldNode::NodeList;

        /// Allows us to build component functionality with composition instead of inheritance
        Core core;

        template <class T>
        SP<T> TypeComponent() const {
            GUARDR(owner, nullptr)
            return owner->TypeComponent<T>();
        }

        template <class T, class ComponentList>
        void TypeComponents(ComponentList& result) const {
            GUARD(owner)
            owner->TypeComponents<T>(result);
        }

        template <class T, class ComponentList>
        void ChildTypeComponents(ComponentList& result) {
            GUARD(owner)
            return owner->ChildTypeComponents<T>(result);
        }

        // Convenience names

        template <class T>
        SP<T> GetComponent() const {
            return TypeComponent<T>();
        }

        template <class T>
        List<SP<T>> GetComponentsInChildren() const {
            List<SP<T>> result;
            GUARDR(owner, result)
            return owner->GetComponentsInChildren<T>();
        }

        template <class T>
        List<SP<T>> GetComponents() const {
            List<SP<T>> result;
            GUARDR(owner, result)
            return owner->GetComponents<T>();
        }

        // MARK: SomeWorldComponent

        void DestroyOwner(float afterSeconds) override {
            GUARD(owner)
            owner->Destroy(afterSeconds);
        }

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
