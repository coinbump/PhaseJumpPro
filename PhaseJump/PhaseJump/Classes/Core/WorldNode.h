#pragma once

#include "CollectionUtils.h"
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

    /**
     Node in a world graph
     Each context/window has its own world, with a world graph
     Each world node can have both child nodes and components

     Each component provides composable logic for their owner node
     */
    class WorldNode : public Base, public Updatable {
    public:
        using Base = PJ::Base;
        using This = WorldNode;
        using ComponentList = List<SP<SomeWorldComponent>>;
        using NodeTransform = WorldNodeTransform;
        using NodeList = List<SP<WorldNode>>;

        friend class World;

    protected:
        ComponentList components;
        float destroyCountdown = 0;
        bool isDestroyed = false;
        bool isActive = true;

        WorldPartLife life;
        WorldNode* parent = nullptr;
        NodeList children;

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

        WorldNode(String name = "");

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

        bool IsActive() const;

        void SetActive(bool isActive);

        void ToggleActive();

        // MARK: Add and remove

        ComponentList const& Components() const;

        void Add(SP<SomeWorldComponent> component);

        template <class T>
        SP<T> AddComponent() {
            auto component = MAKE<T>();
            Add(component);
            return component;
        }

        void Add(SP<WorldNode> node);

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

        void SetScale(Vector3 value);

        void SetScale(float value);

        void SetScale2D(Vector2 value);

        void SetScale2D(float value);

        Vector3 LocalScale() const;

        void SetLocalScale(Vector3 value);

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;
    };
} // namespace PJ
