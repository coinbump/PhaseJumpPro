#pragma once

#include "CollectionUtils.h"
#include "Color.h"
#include "Dev.h"
#include "Identifiable.h"
#include "List.h"
#include "SomeWorldComponent.h"
#include "StandardCore.h"
#include "Tags.h"
#include "TreeNode.h"
#include "Utils.h"
#include "VectorList.h"
#include "WorldComponent.h"
#include "WorldNodeTransform.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/18/24
 */
namespace PJ {
    class World;
    class Matrix4x4;
    class OrthoCamera;
    class Theme;
    class DragHandler2D;
    class Cancellable;

    struct AddChildNodeEvent : public SomeSignal {
        SP<WorldNode> node;

        AddChildNodeEvent(SP<WorldNode> node) :
            node(node) {}
    };

    struct RemoveChildNodeEvent : public SomeSignal {
        SP<WorldNode> node;

        RemoveChildNodeEvent(SP<WorldNode> node) :
            node(node) {}
    };

    /**
     Node in a world graph
     Each context/window has its own world, with a world graph
     Each world node can have both child nodes and components

     Each component provides composable logic for their owner node
     */
    // TODO: re-evaluate use of multiple inheritance here
    class WorldNode : public Base, public Treeable<WorldNode> {
    public:
        using Base = PJ::Base;
        using This = WorldNode;
        using ComponentList = VectorList<SP<SomeWorldComponent>>;
        using NodeTransform = WorldNodeTransform;
        using NodeList = VectorList<SP<WorldNode>>;

        friend class World;

    protected:
        ComponentList components;
        float destroyCountdown = 0;
        bool isDestroyed = false;
        bool isEnabled = true;

        WorldPartLife life;
        // TODO: SP-Audit
        TreeNode<This, SP<This>> tree;

        uint64_t IntIdImpl() const {
            return (uint64_t)this;
        }

    public:
        NodeTransform transform;

    protected:
        class World* world = nullptr;

    public:
        /// Name for browsing and debugging
        String name;

        /// Core properties
        StandardCore core;

        /// Stores subscriptions to reactive values
        UnorderedSet<SP<Cancellable>> cancellables;

        /// Stores timers and animators
        Updatables updatables;

        WorldNode(String name = "");

        // Prevent accidental copies
        DELETE_COPY(WorldNode)

        virtual ~WorldNode() {}

        void Destroy(float countdown = 0);
        void Restore();
        virtual void OnDestroy();

        NodeList const& ChildNodes() const;

        NodeList const& Children() const {
            return tree.Children();
        }

        World* World() const;
        bool IsAwake() const;
        bool IsStarted() const;

        /// Called before Start
        void CheckedAwake();

        /// Called after Awake
        void CheckedStart();

        bool IsDestroyed() const;
        std::size_t ChildCount();

        bool IsEnabled() const;
        This& Enable(bool value);
        void ToggleEnable();

        Matrix4x4 ModelMatrix() const;

        // MARK: Add and remove

        ComponentList const& Components() const;

        void Add(SP<SomeWorldComponent> component);

        template <class T, typename... Arguments>
        auto& AddComponent(Arguments... args) {
            auto component = MAKE<T>(args...);
            Add(component);
            return *component;
        }

        // TODO: unit test
        template <class T, typename... Arguments>
        auto& AddComponentIfNeeded(Arguments... args) {
            auto existingComponent = TypeComponent<T>();
            GUARDR(nullptr == existingComponent, *existingComponent)

            auto component = MAKE<T>(args...);
            Add(component);
            return *component;
        }

        // TODO: unit test
        template <class T, typename... Arguments>
        auto& AddComponentIfNeededWasNeeded(bool& wasNeeded, Arguments... args) {
            auto existingComponent = TypeComponent<T>();
            wasNeeded = false;
            GUARDR(nullptr == existingComponent, *existingComponent)

            wasNeeded = true;

            auto component = MAKE<T>(args...);
            Add(component);
            return *component;
        }

        template <class T, typename... Arguments>
        constexpr auto& With(Arguments... args) {
            return AddComponent<T>(args...);
        }

        template <class T, typename... Arguments>
        constexpr auto& WithId(String id, Arguments... args) {
            auto& result = AddComponent<T>(args...);
            result.id = id;
            return result;
        }

        template <class T, typename... Arguments>
        constexpr auto& WithCore(Arguments... args) {
            return AddComponent<WorldComponent<T>>(args...);
        }

        template <class T, typename... Arguments>
        auto AddComponentPtr(Arguments... args) {
            auto component = MAKE<T>(args...);
            Add(component);
            return component;
        }

        template <class T, typename... Arguments>
        auto WithPtr(Arguments... args) {
            return AddComponentPtr<T>(args...);
        }

        template <class T, typename... Arguments>
        auto WithCorePtr(Arguments... args) {
            return AddComponentPtr<WorldComponent<T>>(args...);
        }

        void Add(SP<WorldNode> node);
        void Insert(SP<WorldNode> node, size_t index);

        template <typename... Arguments>
        WorldNode& AddNode(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return *result;
        }

        template <typename... Arguments>
        constexpr WorldNode& And(Arguments... args) {
            return AddNode(args...);
        }

        template <typename... Arguments>
        SP<WorldNode> AddNodePtr(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return result;
        }

        template <typename... Arguments>
        SP<WorldNode> AndPtr(Arguments... args) {
            return AddNodePtr(args...);
        }

        template <typename... Arguments>
        WorldNode& AddNodeAt(Vector3 pos, Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            result->SetLocalPosition(pos);
            return *result;
        }

        void Remove(WorldNode& node);
        void RemoveAllChildren();
        void Remove(SomeWorldComponent& component);
        void RemoveAllComponents();

        template <class T>
        void RemoveType() {
            auto iterComponents = components;
            std::for_each(
                iterComponents.begin(), iterComponents.end(),
                [&](SP<SomeWorldComponent>& component) {
                    if (Is<T>(component.get())) {
                        Remove(*component);
                    }
                }
            );
        }

        template <class T>
        SP<T> TypeComponent() const {
            for (auto& component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    return typeComponent;
                }
            }

            return nullptr;
        }

        template <class Component>
        bool IsTypeEnabled() const {
            auto i = FirstIf(components, [](auto& component) {
                return Is<Component>(component.get()) && component->IsEnabled();
            });
            return i.has_value();
        }

        template <class Component>
        This& EnableType(bool value) {
            for (auto& component : components) {
                GUARD_CONTINUE(Is<Component>(component.get()))
                component->Enable(value);
            }
            return *this;
        }

        template <class T, class ComponentList>
        void CollectTypeComponents(ComponentList& result) const {
            result.clear();

            for (auto& component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    result.push_back(typeComponent);
                }
            }
        }

        template <class Component>
        VectorList<SP<Component>> TypeComponents() const {
            VectorList<SP<Component>> result;
            CollectTypeComponents<Component>(result);
            return result;
        }

        /// Modify components of this type
        template <class Component>
        void Modify(std::function<void(Component&)> func) {
            auto components = TypeComponents<Component>();
            std::for_each(components.begin(), components.end(), [&](auto& component) {
                func(*component);
            });
        }

        /// Can't be const because of `shared_from_this`
        template <class T, class ComponentList>
        void CollectChildTypeComponents(ComponentList& result) {
            result.clear();

            NodeList graph;
            SP<WorldNode> sharedThis = SCAST<WorldNode>(shared_from_this());
            CollectBreadthFirstTree(sharedThis, graph);

            for (auto& node : graph) {
                auto& components = node->Components();

                for (auto& component : components) {
                    auto typeComponent = DCAST<T>(component);
                    if (typeComponent) {
                        result.push_back(typeComponent);
                    }
                }
            }
        }

        Vector3 LocalToWorld(Vector3 localPos);
        Vector3 WorldToLocal(Vector3 worldPos);

        float Opacity() const;
        This& SetOpacity(float value);

        bool Contains(SomeWorldComponent& value);

        // MARK: Convenience

        template <class T>
        SP<T> GetComponent() const {
            return TypeComponent<T>();
        }

        template <class T>
        VectorList<SP<T>> GetComponents() const {
            VectorList<SP<T>> result;
            CollectTypeComponents<T>(result);

            return result;
        }

        template <class T>
        VectorList<SP<T>> GetComponentsInChildren() {
            VectorList<SP<T>> result;
            CollectChildTypeComponents<T>(result);
            return result;
        }

        Vector3 Scale() const;
        This& SetScale(Vector3 value);
        This& SetScale(float value);
        This& SetScale2D(Vector2 value);
        This& SetScale2D(float value);
        Vector3 LocalScale() const;
        This& SetLocalScale(Vector3 value);

        This& SetLocalPosition(Vector3 position) {
            transform.SetLocalPosition(position);
            return *this;
        }

        This& SetWorldPosition(Vector3 position) {
            transform.SetWorldPosition(position);
            return *this;
        }

        virtual void OnUpdate(TimeSlice time);

        // MARK: Treeable

        WorldNode* Parent() const override {
            return tree.Parent();
        }

        void SetParent(WorldNode* value) override {
            tree.SetParent(value);
        }

    protected:
        void Remove(SP<WorldNode> node);
    };
} // namespace PJ
