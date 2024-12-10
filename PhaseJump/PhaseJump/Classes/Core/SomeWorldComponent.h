#pragma once

#include "Base.h"
#include "Macros.h"
#include "SomeSignal.h"
#include "UIPlan.h"
#include "UITypes.h"
#include "UnorderedMap.h"
#include "Updatables.h"
#include "WorldNodeTransform.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 4 stars
 Needs unit tests
 CODE REVIEW: 8/12/24
 */
namespace PJ {
    // TODO: needs unit tests
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
        using Func = std::function<void(This&)>;
        using SignalFunc = std::function<void(This&, SomeSignal const&)>;
        using PlanUIFunc = std::function<void(This&, String context, UIPlanner&)>;
        using TargetFunc = std::function<WorldNode*(This&)>;

        /// Unique identifier
        String id;

        /// User facing name for display
        String name;

        /// Owner node
        /// Node is responsible for setting this to null when the component is removed
        WorldNode* owner{};

        /// Stores func for component updates
        Updatable updatable;

        /// Stores objects that need time events
        Updatables updatables;

        /// Signal handlers. Mapped by signal id
        UnorderedMap<String, SignalFunc> signalFuncs;

        /// Func to make UI plan for custom UI in editor
        UnorderedMap<String, PlanUIFunc> planUIFuncs;

        Func awakeFunc;
        Func startFunc;
        Func lateUpdateFunc;

        /// Returns the target for this component
        TargetFunc targetFunc;

        /// Called when the componet's enabled state changes
        Func onEnabledChangeFunc;

        SomeWorldComponent(String name = "") :
            name(name) {}

        // Prevent accidental copies
        DELETE_COPY(SomeWorldComponent)

        bool IsEnabled() const {
            return isEnabled;
        }

        void Enable(bool value) {
            GUARD(isEnabled != value)
            isEnabled = value;

            GUARD(onEnabledChangeFunc)
            onEnabledChangeFunc(*this);
        }

        virtual WorldNode* Node() const {
            return owner;
        }

        String Name() const {
            return name.size() > 0 ? name : TypeName();
        }

        String Id() const {
            return id;
        }

        void SetId(String value) {
            id = value;
        }

        /// @return Returns the type name of this component for browsers and debugging
        virtual String TypeName() const = 0;

        void Signal(String id, SomeSignal const& signal);

        Matrix4x4 ModelMatrix() const;

        Vector3 LocalToWorld(Vector3 worldPos) const;
        Vector3 WorldToLocal(Vector3 localPos) const;

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
        virtual void LateUpdate() {
            GUARD(lateUpdateFunc)
            lateUpdateFunc(*this);
        }

        /// Called when node is destroyed
        virtual void OnDestroy() {}

        /// Called in game loop for time delta update events
        virtual void OnUpdate(TimeSlice time) {
            updatable.OnUpdate(time);
            updatables.OnUpdate(time);
        }

        /// @return Returns the target node that this component operates on
        /// In a multi-component system, this will usually be the component's owner
        /// In a single-component per node system (Godot), this could be the owner's parent node
        virtual WorldNode* Target() {
            GUARDR(targetFunc, owner)
            return targetFunc(*this);
        }

        virtual UP<UIPlan> MakeUIPlan(String context);

    protected:
        /// If true, the component should receive events
        bool isEnabled = true;

        WorldPartLife life;

        /// Called before Start by CheckedAwake
        virtual void Awake() {
            GUARD(awakeFunc)
            awakeFunc(*this);
        }

        /// Called after Awake by CheckedStart
        virtual void Start() {
            GUARD(startFunc)
            startFunc(*this);
        }
    };
} // namespace PJ
