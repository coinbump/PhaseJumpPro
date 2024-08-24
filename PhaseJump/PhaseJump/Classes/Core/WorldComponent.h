#pragma once

#include "AcyclicGraphNode.h"
#include "GeoTransform.h"
#include "MultiFunction.h"
#include "SomePosition.h"
#include "SomeWorldComponent.h"
#include "Updatable.h"
#include "Void.h"
#include "WorldNode.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class WorldNode;
    class World;
    template <class Core>
    class WorldComponent;

    // TODO: just give all components this? <- takes some ram but :shrug
    /// Allows function composition as an alternative to inheritance
    struct ComposeWorldComponentCore {
        using Owner = WorldComponent<ComposeWorldComponentCore>;
        using VoidComposeFunctions = MultiFunction<void(Owner&)>;

        VoidComposeFunctions awakeFuncs;
        VoidComposeFunctions startFuncs;
        VoidComposeFunctions lateUpdateFuncs;
        MultiFunction<void(Owner&, TimeSlice)> updateFuncs;
    };

    struct WorldNodeTool {
        template <class T>
        SP<T> TypeComponent(WorldNode* owner) const {
            GUARDR(owner, nullptr)
            return owner->TypeComponent<T>();
        }

        // TODO: use List?
        template <class T>
        VectorList<SP<T>> TypeComponents(WorldNode* owner) const {
            VectorList<SP<T>> result;
            GUARDR(owner, result)
            owner->TypeComponents<T>(result);
            return result;
        }

        template <class T>
        VectorList<SP<T>> ChildTypeComponents(WorldNode* owner) {
            GUARDR(owner, VectorList<SP<T>>())
            return owner->ChildTypeComponents<T>();
        }

        WorldNode::NodeList ChildNodes(WorldNode* owner) const {
            WorldNode::NodeList result;
            GUARDR(owner, result)
            return owner->ChildNodes();
        }
    };

    /// Adds convenience methods that operate on WorldNode
    /// We can't declare these in SomeWorldComponent because it would create a
    /// circular include between SomeWorldComponent and WorldNode
    /// (template funcs can't be moved to the .cpp file to avoid this)
    // TODO: rethink this
    template <class Core = Void>
    class WorldComponent : public SomeWorldComponent {
    public:
        Core core;

        template <class T>
        SP<T> TypeComponent() const {
            return WorldNodeTool().TypeComponent<T>(this->owner);
        }

        template <class T>
        VectorList<SP<T>> TypeComponents() const {
            return WorldNodeTool().TypeComponents<T>(this->owner);
        }

        template <class T>
        VectorList<SP<T>> ChildTypeComponents() {
            return WorldNodeTool().ChildTypeComponents<T>(this->owner);
        }

        List<SP<WorldNode>> ChildNodes() const {
            return WorldNodeTool().ChildNodes(this->owner);
        }

        virtual void UpdateFromSerializedProperties(bool forceUpdate) {}

        // Convenience names
        template <class T>
        SP<T> GetComponent() const {
            return TypeComponent<T>();
        }

        template <class T>
        VectorList<SP<T>> GetComponentsInChildren() const {
            return ChildTypeComponents<T>();
        }

        template <class T>
        VectorList<SP<T>> GetComponents() const {
            return TypeComponents<T>();
        }

        // MARK: SomeWorldComponent

        void DestroyOwner(float afterSeconds) override {
            GUARD(this->owner)
            this->owner->Destroy(afterSeconds);
        }

        // MARK: SomeWorldComponent

        // TODO: Problem: new components can be added dynamically and will need a start and Awake
        // lifecycle event
        void Awake() override {}

        void Start() override {}

        void LateUpdate() override {}

        void OnUpdate(TimeSlice time) override {}
    };

    LocalPosition ScreenToLocal(SomeWorldComponent& component, ScreenPosition screenPos);

    // TODO: should composeCore be in all components by default? Instead of specialization?
    /// Composable action specializations
    template <>
    void WorldComponent<ComposeWorldComponentCore>::Awake();

    template <>
    void WorldComponent<ComposeWorldComponentCore>::Start();
    template <>
    void WorldComponent<ComposeWorldComponentCore>::LateUpdate();
    template <>
    void WorldComponent<ComposeWorldComponentCore>::OnUpdate(TimeSlice time);

} // namespace PJ
