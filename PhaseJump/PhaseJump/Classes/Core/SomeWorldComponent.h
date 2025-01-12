#pragma once

#include "Base.h"
#include "Macros.h"
#include "SomeSignal.h"
#include "UIPlan.h"
#include "UITypes.h"
#include "UnorderedMap.h"
#include "Updatables.h"
#include "WorldAttachmentCore.h"
#include "WorldNodeTransform.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
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

        struct PlanUIArgs {
            This& component;
            String context;
            UIPlanner& planner;
        };

        using PlanUIFunc = std::function<void(PlanUIArgs)>;
        using TargetFunc = std::function<WorldNode*(This&)>;
        using AttachmentCore = WorldAttachmentCore<This>;
        using SignalHandler = AttachmentCore::SignalHandler;
        using SignalFunc = SignalHandler::SignalFunc;

        // Don't allow copies
        DELETE_COPY(SomeWorldComponent)

        AttachmentCore _core;

        /// Owner node
        /// Node is responsible for setting this to null when the component is removed
        WorldNode* owner{};

        /// Func to make UI plan for custom UI in editor
        UnorderedMap<String, PlanUIFunc> planUIFuncs;

        /// Returns the target for this component
        TargetFunc targetFunc;

        SomeWorldComponent(String name = "");

        bool IsEnabled() const;

        This& Enable(bool value);

        virtual WorldNode* Node() const {
            return owner;
        }

        String Name() const {
            return _core.name.size() > 0 ? _core.name : TypeName();
        }

        void SetName(String value) {
            _core.name = value;
        }

        String Id() const {
            return _core.id;
        }

        This& SetId(String value) {
            _core.id = value;
            return *this;
        }

        /// @return Returns the type name of this component for browsers and debugging
        virtual String TypeName() const = 0;

        Matrix4x4 ModelMatrix() const;

        Vector3 LocalToWorld(Vector3 localPos) const;
        Vector3 WorldToLocal(Vector3 worldPos) const;

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

        /// Called in game loop for time delta update events
        virtual void OnUpdate(TimeSlice time) {
            _core.OnUpdate(time);
        }

        /// @return Returns the target node that this component operates on
        /// In a multi-component system, this will usually be the component's owner
        /// In a single-component per node system (Godot), this could be the owner's parent node
        virtual WorldNode* Target() {
            GUARDR(targetFunc, owner)
            return targetFunc(*this);
        }

        virtual UP<UIPlan> MakeUIPlan(String context);

        Updatable& GetUpdatable() {
            return _core.updatable;
        }

        Updatables& GetUpdatables() {
            return _core.updatables;
        }

        /// Adds a signal handler for the specified signal
        This& AddSignalHandler(SignalHandler handler);

        template <class Signal>
        using SignalHandlerConfig = AttachmentCore::SignalHandlerConfig<Signal>;

        template <class Signal>
        This& AddSignalHandler(SignalHandlerConfig<Signal> config) {
            _core.AddSignalHandler<Signal>(config);
            OnAddSignalHandler();
            return *this;
        }

        /// Sends a signal to registered signal handlers
        void Signal(String id, SomeSignal const& signal) {
            _core.Signal(id, signal);
        }

    protected:
        /// If true, the component should receive events
        bool isEnabled = true;

        /// Internal. Tracks the life cycle (Start, Awake) of this object
        WorldPartLife life;

        /// Called when a signal handler is added. Sets the isListener flag on the owner to true
        void OnAddSignalHandler();

        /// Called when the enabled state for this object changes
        virtual void OnEnabledChange() {}

        /// Called before Start by CheckedAwake
        virtual void Awake() {};

        /// Called after Awake by CheckedStart
        virtual void Start() {};
    };
} // namespace PJ
