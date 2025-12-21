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

        // Don't allow copies
        DELETE_COPY(SomeWorldSystem)

        SomeWorldSystem() {}

        virtual ~SomeWorldSystem() {}

        virtual String Name() const = 0;

        virtual void AddTypeTag(String value) = 0;

        virtual bool HasTypeTag(String value) const = 0;

        virtual PJ::World* World() const = 0;

        virtual void SetWorld(class World* value) = 0;

        virtual bool IsAwake() const = 0;

        virtual bool IsStarted() const = 0;

        /// Called before Start
        virtual void CheckedAwake() = 0;

        /// Called after Awake
        virtual void CheckedStart() = 0;

        /// Called when UI events are processed
        virtual void ProcessUIEvents(UIEventList const& uiEvents) {}

        /// Called to render next frame
        virtual std::optional<RenderResult> Render(RenderContextModel& contextModel) {
            return {};
        }

        virtual FinishType OnUpdate(TimeSlice time) {
            return FinishType::Continue;
        }

        /// Calls registered signal handlers for this signal
        virtual void Signal(String id, SomeSignal const& signal) {}
    };

    /// Exists outside of the world node graph and receives UI events
    class WorldSystem : public SomeWorldSystem {
    public:
        using This = WorldSystem;
        using Base = SomeWorldSystem;
        using Func = std::function<void(This&)>;
        using AttachmentCore = WorldAttachmentCore<This>;
        using SignalHandler = AttachmentCore::SignalHandler;
        using SignalFunc = SignalHandler::SignalFunc;

    protected:
        PJ::World* world{};

        WorldPartLife life;

        virtual void Awake() {}

        virtual void Start() {}

        /// Signal handlers. Mapped by signal id
        UnorderedMap<String, VectorList<SignalHandler>> signalHandlers;

        /// Key handlers. Mapped by key + modifiers id
        UnorderedMap<String, Func> keyFuncs;

    public:
        StandardCore _core;
        AttachmentCore attachmentCore;

        WorldSystem(String name = "");

        void AddKeyFunc(int keyCode, VectorList<String> modifiers, Func func);

        template <class Signal>
        using SignalHandlerConfig = AttachmentCore::SignalHandlerConfig<Signal>;

        template <class Signal>
        This& AddSignalHandler(SignalHandlerConfig<Signal> config) {
            attachmentCore.AddSignalHandler<Signal>(config);
            return *this;
        }

        /// Adds a signal handler for the specified signal
        This& AddSignalHandler(SignalHandler handler) {
            attachmentCore.AddSignalHandler(handler);
            return *this;
        }

        // MARK: SomeWorldSystem

        /// Calls registered signal handlers for this signal
        void Signal(String id, SomeSignal const& signal) override {
            attachmentCore.Signal(id, signal);
        }

        String Name() const override {
            return _core.name;
        }

        void AddTypeTag(String value) override {
            _core.typeTags.insert(value);
        }

        bool HasTypeTag(String value) const override {
            return _core.typeTags.contains(value);
        }

        FinishType OnUpdate(TimeSlice time) override {
            return attachmentCore.OnUpdate(time);
        }

        PJ::World* World() const override {
            return world;
        }

        void SetWorld(class World* value) override {
            this->world = value;
        }

        bool IsAwake() const override {
            return life.IsAwake();
        }

        bool IsStarted() const override {
            return life.IsStarted();
        }

        /// Called before Start
        void CheckedAwake() override {
            life.CheckedAwake([this]() { Awake(); });
        }

        /// Called after Awake
        void CheckedStart() override {
            life.CheckedStart([this]() { Start(); });
        }
    };
} // namespace PJ
