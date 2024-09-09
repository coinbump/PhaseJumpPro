#pragma once

#include "CollectionUtils.h"
#include "Color.h"
#include "Identifiable.h"
#include "List.h"
#include "Log.h"
#include "SomeWorldComponent.h"
#include "StandardCore.h"
#include "Tags.h"
#include "Utils.h"
#include "VectorList.h"
#include "WorldNodeTransform.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/18/24
 */
namespace PJ {
    class LocalPosition;
    class World;
    class Matrix4x4;
    class OrthoCamera;
    class Theme;
    class DragHandler2D;
    class Cancellable;

    /**
     Node in a world graph
     Each context/window has its own world, with a world graph
     Each world node can have both child nodes and components

     Each component provides composable logic for their owner node
     */
    class WorldNode : public Base, public Updatable, public IntIdentifiable<WorldNode> {
    public:
        using Base = PJ::Base;
        using This = WorldNode;
        using ComponentList = List<SP<SomeWorldComponent>>;
        using NodeTransform = WorldNodeTransform;
        using NodeList = List<SP<WorldNode>>;
        using OnDragUpdateFunc = std::function<void(DragHandler2D&)>;

        friend class World;
        friend class IntIdentifiable<WorldNode>;

    protected:
        ComponentList components;
        float destroyCountdown = 0;
        bool isDestroyed = false;
        bool isEnabled = true;

        WorldPartLife life;
        WorldNode* parent = nullptr;
        NodeList children;

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

        /// Tags that define the object's type
        TypeTagSet typeTags;

        /// Stores subscriptions to reactive values
        UnorderedSet<SP<Cancellable>> cancellables;

        WorldNode(String name = "");

        // Prevent accidental copies
        DELETE_COPY(WorldNode)

        virtual ~WorldNode() {}

        void Destroy(float countdown = 0);

        virtual void OnDestroy();

        WorldNode* Parent() const;

        NodeList const& ChildNodes() const;
        NodeList const& Children() const;

        bool IsAwake() const;

        bool IsStarted() const;

        World* World() const;

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
        T& AddComponent(Arguments... args) {
            auto component = MAKE<T>(args...);
            Add(component);
            return *component;
        }

        template <class T, typename... Arguments>
        SP<T> AddComponentPtr(Arguments... args) {
            auto component = MAKE<T>(args...);
            Add(component);
            return component;
        }

        void Add(SP<WorldNode> node);

        template <typename... Arguments>
        WorldNode& AddNode(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return *result;
        }

        template <typename... Arguments>
        SP<WorldNode> AddNodePtr(Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            return result;
        }

        template <typename... Arguments>
        WorldNode& AddNodeAt(Vector3 pos, Arguments... args) {
            SP<WorldNode> result = MAKE<WorldNode>(args...);
            Add(result);
            result->SetLocalPosition(pos);
            return *result;
        }

        void Remove(SP<WorldNode> node);

        void RemoveAllChildren();

        void Remove(SP<SomeWorldComponent> component);

        void RemoveAllComponents();

        template <class T>
        SP<T> TypeComponent() const {
            // TODO: use std::find_if (see: TypeSystem)
            for (auto& component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    return typeComponent;
                }
            }

            return nullptr;
        }

        template <class T, class ComponentList>
        void TypeComponents(ComponentList& result) const {
            result.clear();

            // TODO: use std:: func
            for (auto& component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    result.push_back(typeComponent);
                }
            }
        }

        /// Can't be const because of `shared_from_this`
        template <class T, class ComponentList>
        void ChildTypeComponents(ComponentList& result) {
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

        // MARK: Convenience

        template <class T>
        SP<T> GetComponent() const {
            return TypeComponent<T>();
        }

        template <class T>
        List<SP<T>> GetComponents() const {
            List<SP<T>> result;
            TypeComponents<T>(result);

            return result;
        }

        template <class T>
        List<SP<T>> GetComponentsInChildren() {
            List<SP<T>> result;
            ChildTypeComponents<T>(result);
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

        /*
         Quick build methods can be used for rapid prototyping
         */

        // MARK: Quick build

        WorldNode& AddCircle(float radius, Color color = Color::gray);
        WorldNode& AddDrag(OnDragUpdateFunc onDragUpdateFunc);
        WorldNode& AddSquareCollider(float size);
        WorldNode& AddRectCollider(Vector2 size);
        WorldNode& AddCircleCollider(float radius);
        WorldNode& AddCircleHandle(
            float radius, Color color = Color::gray, float strokeWidth = 4,
            Color strokeColor = Color::black
        );
        WorldNode& AddCircleHandle(float radius, float strokeWidth = 4, Theme* theme = nullptr);
        WorldNode& AddSlider(String name, Vector2 worldSize, std::function<void(float)> valueFunc);
        WorldNode&
        AddSliderVertical(String name, Vector2 worldSize, std::function<void(float)> valueFunc);

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;
    };
} // namespace PJ
