#ifndef PJWORLDNODE_H
#define PJWORLDNODE_H

#include "AcyclicGraphNode.h"
#include "SomeWorldComponent.h"
#include "List.h"
#include "GeoTransform.h"
#include <memory>

namespace PJ {
    /// <summary>
    /// A node in the world graph
    /// </summary>
    class WorldNode : public AcyclicGraphNode<> {
    public:
        using ComponentSharedPtr = std::shared_ptr<SomeWorldComponent>;
        using ComponentPtr = ComponentSharedPtr const&;

    protected:
        List<ComponentSharedPtr> components;
        float destroyCountdown = 0;
        bool isDestroyed = false;

    public:
        using Base = AcyclicGraphNode<>;
        std::shared_ptr<GeoTransform> transform = std::make_shared<GeoTransform>();

        bool IsDestroyed() const { return isDestroyed; }

        List<ComponentSharedPtr> const& Components() const { return components; }

        void AddComponent(ComponentPtr component)
        {
            if (!component) { return; }
            components.Add(component);
            component->owner = static_pointer_cast<WorldNode>(shared_from_this());
        }

        template <class T>
        std::shared_ptr<T> GetComponent() const
        {
            for (auto component : components) {
                auto typeComponent = dynamic_pointer_cast<T>(component);
                if (typeComponent) {
                    return typeComponent;
                }
            }

            return nullptr;
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

        std::shared_ptr<List<std::shared_ptr<WorldNode>>> ChildNodes() const
        {
            auto result = std::make_shared<List<std::shared_ptr<WorldNode>>>();

            for (auto edge : edges) {
                auto worldNode = dynamic_pointer_cast<WorldNode>(edge->toNode->Value());
                if (worldNode) {
                    result->Add(worldNode);
                }
            }

            return result;
        }
    };
}

#endif
