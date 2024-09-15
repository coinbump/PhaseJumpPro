#pragma once

#include "Base.h"
#include "Macros.h"
#include "SomeSignal.h"
#include "UnorderedMap.h"
#include "Updatable.h"
#include "Updatables.h"
#include "WorldNodeTransform.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/12/24
 */
namespace PJ {
    class World;
    class WorldNode;
    class Matrix4x4;

    /**
     Interface for a world component

     A world consists of a tree of world nodes. Each world node can have 0-N components

     Each component extends the functionality of the owner node with custom behavior.
     A component receives the same lifecycle events as its owner
     */
    class SomeWorldComponent : public Base, public Updatable {
    protected:
        /// If true, the component should receive events
        bool isEnabled = true;

        WorldPartLife life;

        /// Called before Start by CheckedAwake
        virtual void Awake() {}

        /// Called after Awake by CheckedStart
        virtual void Start() {}

    public:
        using This = SomeWorldComponent;
        using NodeTransform = WorldNodeTransform;
        using SignalFunc = std::function<void(This&, SomeSignal const&)>;

        /// Owner node
        /// Node is responsible for setting this to null when the component is removed
        WorldNode* owner = nullptr;

        /// Add objects that need time updates here: timers, animations, etc.
        Updatables updatables;

        /// Signal handlers. Mapped by signal id
        UnorderedMap<String, SignalFunc> signalHandlers;

        SomeWorldComponent() {}

        // Prevent accidental copies
        DELETE_COPY(SomeWorldComponent)

        bool IsEnabled() const {
            return isEnabled;
        }

        virtual WorldNode* Node() const {
            return owner;
        }

        /// Returns the type name of this component for browsers and debugging
        virtual String TypeName() const = 0;

        Matrix4x4 ModelMatrix() const;

        // MARK: Lifecycle events

        bool IsAwake() const {
            return life.IsAwake();
        }

        bool IsStarted() const {
            return life.IsStarted();
        }

        /// Called before Start
        void CheckedAwake();

        /// Called after Awake
        void CheckedStart();

        /// Called after OnUpdate
        virtual void LateUpdate() {}

        /// Called when node is destroyed
        virtual void OnDestroy() {}

        // MARK: Updatable

        /// Called in game loop for time delta update events
        void OnUpdate(TimeSlice time) override {
            updatables.OnUpdate(time);
        }
    };
} // namespace PJ
