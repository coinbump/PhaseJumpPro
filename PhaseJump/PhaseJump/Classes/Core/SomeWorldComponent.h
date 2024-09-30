#pragma once

#include "Base.h"
#include "Macros.h"
#include "SomeSignal.h"
#include "UIPlan.h"
#include "UnorderedMap.h"
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
    class UIPlan;
    class UIPlanner;

    /**
     Interface for a world component

     A world consists of a tree of world nodes. Each world node can have 0-N components

     Each component extends the functionality of the owner node with custom behavior.
     A component receives the same lifecycle events as its owner
     */
    class SomeWorldComponent : public Base, public SomeUpdatable {
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
        using PlanUIFunc = std::function<void(This&, String context, UIPlanner&)>;

        String name;

        /// Owner node
        /// Node is responsible for setting this to null when the component is removed
        WorldNode* owner = nullptr;

        /// Stores objects that need time events
        Updatables updatables;

        /// Signal handlers. Mapped by signal id
        UnorderedMap<String, SignalFunc> signalHandlers;

        /// (Optional). Func to make UI plan for custom UI in editor
        PlanUIFunc planUIFunc;

        SomeWorldComponent(String name = "") :
            name(name) {}

        // Prevent accidental copies
        DELETE_COPY(SomeWorldComponent)

        bool IsEnabled() const {
            return isEnabled;
        }

        void SetIsEnabled(bool value) {
            isEnabled = value;
        }

        void Enable() {
            SetIsEnabled(true);
        }

        virtual WorldNode* Node() const {
            return owner;
        }

        String Name() const {
            return name.size() > 0 ? name : TypeName();
        }

        /// @return Returns the type name of this component for browsers and debugging
        virtual String TypeName() const = 0;

        void Signal(String id, SomeSignal const& signal);

        Matrix4x4 ModelMatrix() const;

        Vector3 LocalToWorld(Vector3 worldPos);
        Vector3 WorldToLocal(Vector3 localPos);

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

        // MARK: SomeUpdatable

        /// Called in game loop for time delta update events
        void OnUpdate(TimeSlice time) override {
            updatables.OnUpdate(time);
        }

        bool IsFinished() const override {
            return false;
        }

        /// (Optional). Produce a UI plan to add custom UI when inspecting this component
        /// WARNING. Apple clang C++ compiler breaks `dynamic_cast` if this function is virtual
        /// and enable testability is off.
        /// Enable Testability must be on for `dynamic_cast` to work, until Apple fixes the bug
        UP<UIPlan> MakeUIPlan(String context);
    };
} // namespace PJ
