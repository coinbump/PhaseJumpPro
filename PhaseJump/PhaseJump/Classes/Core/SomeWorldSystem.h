#pragma once

#include "Base.h"
#include "List.h"
#include "SomeUIEvent.h"
#include "SomeWorldComponent.h"
#include "Tags.h"
#include "Updatable.h"
#include "VectorList.h"
#include "WorldAttachmentCore.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    class World;
    class RenderContextModel;

    /// Stores information from the render system used for logging and dev views
    struct RenderResult {
        Tags tags;
    };

    /// Exists outside of the world node graph and receives UI events
    class SomeWorldSystem : public Base {
    public:
        using This = SomeWorldSystem;
        using Func = std::function<void(This&)>;
        using AttachmentCore = WorldAttachmentCore<This>;
        using SignalHandler = AttachmentCore::SignalHandler;
        using SignalFunc = SignalHandler::SignalFunc;

        // Don't allow copies
        DELETE_COPY(SomeWorldSystem)

    protected:
        World* world{};

        WorldPartLife life;

        virtual void Awake() {}

        virtual void Start() {}

        /// Signal handlers. Mapped by signal id
        UnorderedMap<String, VectorList<SignalHandler>> signalHandlers;

        /// Key handlers. Mapped by key + modifiers id
        UnorderedMap<String, Func> keyFuncs;

    public:
        AttachmentCore _core;

        /// Object type attributes
        TypeTagSet typeTags;

        SomeWorldSystem(String name = "");

        virtual ~SomeWorldSystem() {}

        World* World() const {
            return world;
        }

        void SetWorld(class World* value) {
            this->world = value;
        }

        String Name() const {
            return _core.name;
        }

        // MARK: Lifecycle events

        bool IsAwake() const {
            return life.IsAwake();
        }

        bool IsStarted() const {
            return life.IsStarted();
        }

        /// Called before Start
        void CheckedAwake() {
            life.CheckedAwake([this]() { Awake(); });
        }

        /// Called after Awake
        void CheckedStart() {
            life.CheckedStart([this]() { Start(); });
        }

        /// Called when UI events are processed
        virtual void ProcessUIEvents(UIEventList const& uiEvents) {}

        /// Called to render next frame
        virtual std::optional<RenderResult> Render(RenderContextModel& contextModel) {
            return std::nullopt;
        }

        void AddKeyFunc(int keyCode, VectorList<String> modifiers, Func func);

        /// Calls registered signal handlers for this signal
        void Signal(String id, SomeSignal const& signal) {
            _core.Signal(id, signal);
        }

        /// Adds a signal handler for the specified signal
        This& AddSignalHandler(SignalHandler handler) {
            _core.AddSignalHandler(handler);
            return *this;
        }

        template <class Signal>
        using SignalHandlerConfig = AttachmentCore::SignalHandlerConfig<Signal>;

        template <class Signal>
        This& AddSignalHandler(SignalHandlerConfig<Signal> config) {
            _core.AddSignalHandler<Signal>(config);
            return *this;
        }

        virtual void OnUpdate(TimeSlice time) {
            _core.OnUpdate(time);
        }
    };
} // namespace PJ
