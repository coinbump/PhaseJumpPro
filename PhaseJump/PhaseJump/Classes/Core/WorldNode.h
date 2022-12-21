#ifndef PJWORLDNODE_H
#define PJWORLDNODE_H

#include "AcyclicGraphNode.h"
#include "WorldComponent.h"
#include "List.h"
#include <memory>

namespace PJ {
    /// <summary>
    /// A node in the world graph
    /// </summary>
    class WorldNode : public AcyclicGraphNode<> {
    public:
        using ComponentSharedPtr = std::shared_ptr<WorldComponent>;
        using ComponentPtr = ComponentSharedPtr const&;

    protected:
        List<ComponentSharedPtr> components;
        float destroyCountdown = 0;
        bool isDestroyed = false;

    public:
        using Base = AcyclicGraphNode<>;

        bool IsDestroyed() const { return isDestroyed; }

        void Destroy(float countdown = 0)
        {
            if (countdown == 0) {
                isDestroyed = true;
                return;
            }

            destroyCountdown = countdown;
        }

        void AddComponent(ComponentPtr component) {
            components.Add(component);
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
    };
}

#endif
