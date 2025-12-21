#pragma once

#include "Base.h"
#include "Macros.h"
#include "SomeSignal.h"
#include "StandardCore.h"
#include "UIPlan.h"
#include "UITypes.h"
#include "UnorderedMap.h"
#include "Updatables.h"
#include "WorldAttachmentCore.h"
#include "WorldNodeTransform.h"
#include <memory>

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/20/25
 */
namespace PJ {
    class World;
    class WorldNode;
    class Matrix4x4;
    class UIPlan;
    class UIPlanner;

    /**
     Interface for a world component

     A world consists of a tree of world nodes. Each world node can have 0-N components

     Each component extends the functionality of the owner node with custom behavior.
     A component receives the same lifecycle events as its owner
     */
    class SomeWorldComponent : public Base {
    public:
        using This = SomeWorldComponent;
        using NodeTransform = WorldNodeTransform;
        using SignalHandler = PJ::SignalHandler<This>;
        using SignalFunc = SignalHandler::SignalFunc;

        // Don't allow copies
        DELETE_COPY(SomeWorldComponent)

        SomeWorldComponent() {}

        virtual ~SomeWorldComponent() {}

        /// @return Returns true if this component is enabled
        virtual bool IsEnabled() const = 0;

        /// Sets the enabled state
        virtual SomeWorldComponent& Enable(bool value) = 0;

        /// @return Returns the owner node
        virtual WorldNode* Node() const = 0;

        /// Assigns the owner node
        virtual SomeWorldComponent& SetNode(WorldNode* value) = 0;

        /// @return Returns the browsing name
        virtual String Name() const = 0;

        /// Sets the browsing name
        virtual SomeWorldComponent& SetName(String value) = 0;

        /// @return Returns a unique identifier (implementation dependent)
        virtual String Id() const = 0;

        /// Assigns a unique identifier
        virtual SomeWorldComponent& SetId(String value) = 0;

        /// @return Returns the type name of this component for browsers and debugging
        virtual String TypeName() const = 0;

        /// @return Returns the owner node's model matrix
        Matrix4x4 ModelMatrix() const;

        /// @return Returns local position in world coordinates
        Vector3 LocalToWorld(Vector3 localPos) const;

        /// @return Returns world position in local coordinates
        Vector3 WorldToLocal(Vector3 worldPos) const;

        // MARK: Lifecycle events

        /// @return Returns true if component is awake
        virtual bool IsAwake() const = 0;

        /// @return Returns true if component is started
        virtual bool IsStarted() const = 0;

        /// Called before Start
        virtual void CheckedAwake() = 0;

        /// Called after Awake
        virtual void CheckedStart() = 0;

        /// Called after OnUpdate
        virtual void LateUpdate() {}

        /// Called when node is destroyed
        virtual void OnDestroy() {}

        /// Called in game loop for time delta update events
        virtual FinishType OnUpdate(TimeSlice time) = 0;

        /// @return Returns the target node that this component operates on
        /// In a multi-component system, this will usually be the component's owner
        /// In a single-component per node system (Godot), this could be the owner's parent node
        virtual WorldNode* Target() {
            return Node();
        }

        /// @return Returns an optional UI plan used to build inspectors for this component
        virtual UP<UIPlan> MakeUIPlan(String context) {
            return {};
        }

        virtual Updatable& GetUpdatable() = 0;

        virtual Updatables& GetUpdatables() = 0;

        /// Adds a signal handler for the specified signal
        virtual This& AddSignalHandler(SignalHandler handler) = 0;

        /// Sends a signal to registered signal handlers
        virtual void Signal(String id, SomeSignal const& signal) = 0;

        /// @return Returns true if the component listens to this signal
        virtual bool IsListenerToSignal(String id) const = 0;

        /// @return Returns true if the component is registered to listen to signals
        virtual bool IsListener() const = 0;

    protected:
        /// Called before Start by CheckedAwake
        virtual void Awake() {};

        /// Called after Awake by CheckedStart
        virtual void Start() {};
    };
} // namespace PJ
