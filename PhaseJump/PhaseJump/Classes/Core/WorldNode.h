#ifndef PJWORLDNODE_H
#define PJWORLDNODE_H

#include "AcyclicGraphNode.h"
#include "SomeWorldComponent.h"
#include "VectorList.h"
#include "List.h"
#include "GeoTransform.h"
#include "SomeUIEvent.h"
#include "Macros.h"
#include <memory>

namespace PJ {
    class LocalPosition;
    class World;

    /// <summary>
    /// A node in the world graph
    /// </summary>
    class WorldNode : public AcyclicGraphNode<> {
    public:
        using ComponentSharedPtr = SP<SomeWorldComponent>;
        using ComponentPtr = ComponentSharedPtr const&;

    protected:
        VectorList<ComponentSharedPtr> components;
        float destroyCountdown = 0;
        bool isDestroyed = false;
        bool isActive = true;

    public:
        std::weak_ptr<World> world;

        using Base = AcyclicGraphNode<>;
        SP<GeoTransform> transform = MAKE<GeoTransform>();

        bool IsDestroyed() const { return isDestroyed; }
        std::size_t ChildCount() { return edges.size(); }

        VectorList<ComponentSharedPtr> const& Components() const { return components; }

        std::weak_ptr<World> World() const;

        void Add(ComponentPtr component)
        {
            if (!component) { return; }
            components.Add(component);
            component->owner = SCAST<WorldNode>(shared_from_this());
        }

        template <class T>
        bool AddComponent(SP<T> component)
        {
            auto castComponent = DCAST<SomeWorldComponent>(component);
            if (nullptr == castComponent) { return false; }
            Add(castComponent);

            return true;
        }

        template <class T>
        SP<T> AddComponent()
        {
            auto component = MAKE<T>();
            AddComponent(component);
            return component;
        }

        void AddChild(SP<WorldNode> node)
        {
            AddEdge(StandardEdgeModel(), node);
        }

        template <class T>
        SP<T> TypeComponent() const
        {
            for (auto component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    return typeComponent;
                }
            }

            return nullptr;
        }

        template <class T>
        VectorList<SP<T>> TypeComponents() const
        {
            VectorList<SP<T>> result;

            for (auto component : components) {
                auto typeComponent = DCAST<T>(component);
                if (typeComponent) {
                    result.Add(typeComponent);
                }
            }

            return result;
        }

        void Destroy(float countdown = 0)
        {
            if (countdown == 0) {
                isDestroyed = true;
                return;
            }

            destroyCountdown = countdown;
        }

        /// Called for every object before Start
        virtual void Awake() {
            for (auto component : components) {
                component->Awake();
            }
        }

        /// Called for every object after Awake
        virtual void Start() {
            for (auto component : components) {
                component->Start();
            }
        }

        bool IsActive() const { return isActive; }
        void SetActive(bool isActive) { this->isActive = isActive; }

        virtual void OnDestroy() {}

        void OnUpdate(TimeSlice time) override
        {
            Base::OnUpdate(time);

            if (destroyCountdown > 0) {
                destroyCountdown -= time.delta;
                if (destroyCountdown <= 0) {
                    isDestroyed = true;
                }
            }

            for (auto component : components) {
                if (!component->IsEnabled()) { continue; }
                component->OnUpdate(time);
            }
        }

        VectorList<SP<WorldNode>> ChildNodes() const
        {
            VectorList<SP<WorldNode>> result;

            for (auto edge : edges) {
                auto worldNode = SCAST<WorldNode>(edge->toNode->Value());
                if (worldNode) {
                    result.Add(worldNode);
                }
            }

            return result;
        }

        // Convenience names
        template <class T>
        SP<T> GetComponent() const { return TypeComponent<T>(); }
        template <class T>
        VectorList<SP<T>> GetComponents() const { return TypeComponents<T>(); }

        // TODO: send events to components
        virtual void OnPointerDownEvent(PointerDownUIEvent<LocalPosition> event) {}
    };
}

#endif
