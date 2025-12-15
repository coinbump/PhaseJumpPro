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
    class WorldNode : public Base, public Treeable<WorldNode> {
    public:
        using Base = PJ::Base;
        using This = WorldNode;
        using ComponentList = VectorList<SP<SomeWorldComponent>>;
        using NodeTransform = WorldNodeTransform;
        using NodeList = VectorList<SP<WorldNode>>;

        friend class World;

        // Don't allow copies
        DELETE_COPY(WorldNode)

    protected:
        ComponentList components;
        float destroyCountdown{};
        bool isDestroyed{};

        /// If true, this node will not receive signals and updates
        bool isDisabled{};

        /// If true, this node will not be rendererd
        bool isInvisible{};

        WorldPartLife life;
        TreeNode<This, SP<This>> tree;

        uint64_t IntIdImpl() const {
            return (uint64_t)this;
        }

        /// Optimize: Used to map child nodes by id
        UnorderedMap<String, This*> childMap;

    public:
        NodeTransform transform;

    protected:
        class World* world = nullptr;

    public:
        /// Core properties
        StandardCore core;

        /// If true, this node will receive signals
        bool isListener{};

        /// Stores subscriptions to reactive values
        UnorderedSet<SP<Cancellable>> cancellables;

        /// Stores timers and animators
        Updatables updatables;

        struct Config {
            String id;

            String name;
        };

        WorldNode(Config const& config);
        WorldNode(String name = "");

        virtual ~WorldNode() {}

        /// Destroy this node in the next update, or after a countdown
        void Destroy(float countdown = 0);

        /// Restore a destroyed object (for undo)
        void Restore();

        /// @return Returns the list of child nodes
        NodeList const& ChildNodes() const;

        /// @return Returns the list of child nodes
        NodeList const& Children() const {
            return tree.Children();
        }

        /// @return Returns the node's name
        String Id() const {
            return core.id;
        }

        /// Sets the nodes's name
        void SetId(String value) {
            core.id = value;
        }

        /// @return Returns the node's name
        String Name() const {
            return core.name;
        }

        /// Sets the nodes's name
        void SetName(String value) {
            core.name = value;
        }

        /// @return Returns the number of child nodes this node contains
        std::size_t ChildCount();

        /// @return Returns a reference to the world that owns this node
        World* World() const;

        /// @return Returns true if the node has received the Awake event
        bool IsAwake() const;

        /// @return Returns true if the node has received the Start event
        bool IsStarted() const;

        /// Send a signal to this node and its components
        void Signal(String signalId, SomeSignal const& signal);

        /// Called before Start
        void CheckedAwake();

        /// Called after Awake
        void CheckedStart();

        /// @return Returns true if the node is marked to be destroyed
        bool IsDestroyed() const;

        /// Called by world when object is destroyed
        virtual void OnDestroy();

        /// @return Returns true if the node is enabled for events, interactions, etc.
        bool IsEnabled() const;

        /// Enable or disable the node. Disabled nodes receive no time or signal events
        This& Enable(bool value);

        /// Toggle the node's enabled state. Disabled nodes receive no time or signal events
        void ToggleEnable();

        /// @return Returns true if the node is visible
        bool IsVisible() const;

        /// Shows or hides the node
        WorldNode& SetIsVisible(bool value);

        /// Shows the node
        WorldNode& Show() {
            return SetIsVisible(true);
        }

        /// Hides the node
        WorldNode& Hide() {
            return SetIsVisible(false);
        }

        /// Toggles the node's visibility state
        void ToggleIsVisible();

        /// @return Returns a matrix for transforming from model to world space
        Matrix4x4 ModelMatrix() const;

        /// @return Returns the node's components
        ComponentList const& Components() const;

        /// Adds a component
        void Add(SP<SomeWorldComponent> component);

        /// Adds an associate component (implementation depends on single vs multiple component)
        void AddAssociate(SP<SomeWorldComponent> component);

        /// Adds a component based on type and arguments
        /// @return Returns a reference to the component
        template <class Type, typename... Arguments>
        auto& AddComponent(Arguments... args) {
            auto component = MAKE<Type>(args...);
            Add(component);
            return *component;
        }

        /// Adds a component if needed. If one of this type already exists, it is returned
        /// @return Returns a reference to the component
        template <class Type, typename... Arguments>
        auto& AddComponentIfNeeded(Arguments... args) {
            auto existingComponent = TypeComponent<Type>();
            GUARDR(nullptr == existingComponent, *existingComponent)

            auto component = MAKE<Type>(args...);
            Add(component);
            return *component;
        }

        /// Adds a component if needed. If one of this type already exists, it is returned
        /// Sets was needed flag to true if a new component was added
        /// @return Returns a reference to the component
        template <class Type, typename... Arguments>
        auto& AddComponentIfNeededWasNeeded(bool& wasNeeded, Arguments... args) {
            auto existingComponent = TypeComponent<Type>();
            wasNeeded = false;
            GUARDR(nullptr == existingComponent, *existingComponent)

            wasNeeded = true;

            auto component = MAKE<Type>(args...);
            Add(component);
            return *component;
        }

        /// Quick build for adding a component with type and arguments
        /// @return Returns a reference to the component
        template <class Type, typename... Arguments>
        constexpr auto& With(Arguments... args) {
            return AddComponent<Type>(args...);
        }

        /// Quick build for adding a component with id, type, and arguments
        /// @return Returns a reference to the component
        template <class Type, typename... Arguments>
        constexpr auto& WithId(String id, Arguments... args) {
            auto& result = AddComponent<Type>(args...);
            result.id = id;
            return result;
        }

        /// Quick build for adding a component with core, type, and arguments
        /// @return Returns a reference to the component
        template <class Type, typename... Arguments>
        constexpr auto& WithCore(Arguments... args) {
            return AddComponent<WorldComponent<Type>>(args...);
        }

        /// Adds a component based on type and arguments
        /// @return Returns the component pointer
        template <class Type, typename... Arguments>
        auto AddComponentPtr(Arguments... args) {
            auto component = MAKE<Type>(args...);
            Add(component);
            return component;
        }

        /// Quick build for adding a component with type and arguments
        /// @return Returns the component pointer
        template <class Type, typename... Arguments>
        auto WithPtr(Arguments... args) {
            return AddComponentPtr<Type>(args...);
        }

        /// Quick build for adding a component with core, type, and arguments
        /// @return Returns the component pointer
        template <class Type, typename... Arguments>
        auto WithCorePtr(Arguments... args) {
            return AddComponentPtr<WorldComponent<Type>>(args...);
        }

        /// Adds a child node
        void Add(SP<WorldNode> node);

        /// Insert a child node at the specified index (for redo)
        void Insert(SP<WorldNode> node, size_t index);

        /// Adds a child node based on arguments
        /// @return Returns a reference to the node
        template <typename... Arguments>
        WorldNode& AddNode(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return *result;
        }

        /// Quick build for adding a child node based on arguments
        /// @return Returns a reference to the node
        template <typename... Arguments>
        constexpr WorldNode& And(Arguments... args) {
            return AddNode(args...);
        }

        /// Adds a child node based on arguments
        /// @return Returns the node pointer
        template <typename... Arguments>
        SP<WorldNode> AddNodePtr(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return result;
        }

        /// Quick build for adding a child node based on arguments
        /// @return Returns the node pointer
        template <typename... Arguments>
        SP<WorldNode> AndPtr(Arguments... args) {
            return AddNodePtr(args...);
        }

        /// Quick build for adding a child node based on position and arguments
        /// @return Returns the node pointer
        template <typename... Arguments>
        WorldNode& AddNodeAt(Vector3 pos, Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            result->SetLocalPosition(pos);
            return *result;
        }

        /// Moves ownership of this node away from the parent
        SP<WorldNode> Move(WorldNode& node);

        /// Removes child immediately if allowed. Can fail
        void Remove(WorldNode& node);

        /// Removes all children immediately if allowed. Can fail
        void RemoveAllChildren();

        /// Marks all children to be destroyed when it is safe to do so
        void DestroyAllChildren();

        /// Removes the specified component
        void Remove(SomeWorldComponent& component);

        /// Removes all components
        void RemoveAllComponents();

        /// Removes all components of the specified type
        template <class Type>
        void RemoveType() {
            auto iterComponents = components;
            std::for_each(
                iterComponents.begin(), iterComponents.end(),
                [&](SP<SomeWorldComponent>& component) {
                    if (Is<Type>(component.get())) {
                        Remove(*component);
                    }
                }
            );
        }

        /// @return Returns the first component of the specified type, if any exists
        template <class Type>
        Type* TypeComponent() const {
            for (auto& component : components) {
                auto typeComponent = dynamic_cast<Type*>(component.get());
                GUARD_CONTINUE(typeComponent)

                return typeComponent;
            }

            return nullptr;
        }

        /// @return Returns true if any component of the specified type is enabled
        template <class Type>
        bool IsTypeEnabled() const {
            auto i = FirstIf(components, [](auto& component) {
                return Is<Type>(component.get()) && component->IsEnabled();
            });
            return i.has_value();
        }

        /// Updates enabled value for all components of the specified type
        template <class Type>
        This& EnableType(bool value) {
            for (auto& component : components) {
                GUARD_CONTINUE(Is<Type>(component.get()))
                component->Enable(value);
            }
            return *this;
        }

        /// Collects all components of the specified type
        template <class Type, class ResultList>
        void CollectTypeComponents(ResultList& result) const {
            for (auto& component : components) {
                auto typeComponent = dynamic_cast<Type*>(component.get());
                GUARD_CONTINUE(typeComponent)
                result.push_back(typeComponent);
            }
        }

        /// Collects all components of the specified type, based on a predicate check
        template <class Type, class ResultList, class UnaryPred>
        void CollectTypeComponentsIf(ResultList& result, UnaryPred filter) const {
            for (auto& component : components) {
                auto typeComponent = dynamic_cast<Type*>(component.get());
                GUARD_CONTINUE(typeComponent)
                GUARD_CONTINUE(filter(*typeComponent))
                result.push_back(typeComponent);
            }
        }

        /// @return Returns all components of the specified type
        template <class Type>
        VectorList<Type*> TypeComponents() const {
            VectorList<Type*> result;
            CollectTypeComponents<Type>(result);
            return result;
        }

        /// Call a modify function for each component of this type
        template <class Type>
        void Modify(std::function<void(Type&)> func) {
            auto components = TypeComponents<Type>();
            std::for_each(components.begin(), components.end(), [&](auto& component) {
                func(*component);
            });
        }

        /// Collects components of the specified type in all descendant nodes
        template <class Type, class ResultList, class UnaryPred>
        void CollectDescendantTypeComponentsIf(ResultList& result, UnaryPred filter) {
            VectorList<WorldNode*> graph;
            CollectBreadthFirstTree(this, graph);

            for (auto& node : graph) {
                node->CollectTypeComponentsIf<Type>(result, filter);
            }
        }

        /// Collects components of the specified type in direct child nodes
        template <class Type, class ResultList, class UnaryPred>
        void CollectChildTypeComponentsIf(ResultList& result, UnaryPred filter) {
            for (auto& node : tree.Children()) {
                node->CollectTypeComponentsIf<Type>(result, filter);
            }
        }

        /// Collects components of the specified type in node and direct child nodes
        template <class Type, class ResultList, class UnaryPred>
        void CollectAssociateTypeComponentsIf(ResultList& result, UnaryPred filter) {
            CollectTypeComponentsIf<Type>(result, filter);
            CollectChildTypeComponentsIf<Type>(result, filter);
        }

        /// @return Returns converted local point in world coordinates
        Vector3 LocalToWorld(Vector3 localPos);

        /// @return Returns converted world point in local coordinates
        Vector3 WorldToLocal(Vector3 worldPos);

        /// @return Returns the opacity of an attached renderer
        float Opacity() const;

        /// Sets the opacity of an attached renderer
        This& SetOpacity(float value);

        /// @return Returns true if value is one of this node's components
        bool Contains(SomeWorldComponent& value);

        // MARK: Convenience

        /// @return Returns the component of the specified type, if any
        template <class Type>
        Type* GetComponent() const {
            return TypeComponent<Type>();
        }

        /// @return Returns a list of components of the specified type
        template <class Type>
        VectorList<Type*> GetComponents() const {
            VectorList<Type*> result;
            CollectTypeComponents<Type>(result);

            return result;
        }

        /// @return Returns a list of components of the specified type in all descendant nodes
        template <class Type>
        VectorList<Type*> GetDescendantComponents() {
            VectorList<Type*> result;
            CollectDescendantTypeComponentsIf<Type>(result, [](auto& element) { return true; });
            return result;
        }

        /// @return Returns current scale for node
        Vector3 Scale() const;

        /// Sets the transform scale for the node
        This& SetScale(Vector3 value);

        /// Sets the uniform transform scale for the node
        This& SetScale(float value);

        /// Sets the transform scale for the node and keeps z as 0
        This& SetScale2D(Vector2 value);

        /// Sets the uniform transform scale for the node and keeps z as 0
        This& SetScale2D(float value);

        /// Sets the local transform position for the node
        This& SetLocalPosition(Vector3 position) {
            transform.SetLocalPosition(position);
            return *this;
        }

        /// Sets the world transform position for the node
        This& SetWorldPosition(Vector3 position) {
            transform.SetWorldPosition(position);
            return *this;
        }

        /// Called for update events
        virtual FinishType OnUpdate(TimeSlice time);

        /// Called after every node receives OnUpdate
        virtual void LateUpdate();

        /// @return Returns the child mapped by id
        This* operator[](String id) {
            try {
                return childMap.at(id);
            } catch (...) {
                return {};
            }
        }

        // MARK: Treeable

        WorldNode* Parent() const override {
            return tree.Parent();
        }

        void SetParent(WorldNode* value) override {
            tree.SetParent(value);
        }

    protected:
        /// Removes the specified node
        void Remove(SP<WorldNode> node);
    };

    /// @return Returns the sibling components of the specified type
    template <class Type>
    VectorList<Type*> SiblingTypeComponents(SomeWorldComponent& component) {
        GUARDR(component.owner, {})

        VectorList<Type*> result;
        component.owner->CollectTypeComponentsIf<Type>(result, [&](Type& sibling) {
            return &sibling != &component;
        });

        return result;
    }
} // namespace PJ
