#pragma once

#include "SomePosition.h"
#include "SomeWorldComponent.h"
#include "StandardCore.h"
#include "UIPlanner.h"
#include "Void.h"
#include "WorldAttachmentCore.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// World component with a core
    class WorldComponent : public SomeWorldComponent {
    public:
        using Base = SomeWorldComponent;
        using WorldComponentFunc = std::function<void(This&)>;
        using TargetFunc = std::function<WorldNode*(This&)>;
        using AttachmentCore = WorldAttachmentCore<This>;

        template <class Signal>
        using SignalHandlerConfig = AttachmentCore::SignalHandlerConfig<Signal>;

        struct PlanUIArgs {
            This& component;
            String context;
            UIPlanner& planner;
        };

        using PlanUIFunc = std::function<void(PlanUIArgs)>;

        /// Standard core properties
        StandardCore _core;

        AttachmentCore attachmentCore;

        /// Func to make UI plan for custom UI in editor
        UnorderedMap<String, PlanUIFunc> planUIFuncs;

        /// Returns the target for this component
        TargetFunc targetFunc;

        WorldComponentFunc awakeFunc;
        WorldComponentFunc startFunc;
        WorldComponentFunc lateUpdateFunc;

        /// Owner node
        /// Node is responsible for setting this to null when the component is removed
        WorldNode* owner{};

        WorldComponent(String name = "");

        // FUTURE: virtual void UpdateFromSerializedProperties(bool forceUpdate) {}

        template <class Signal>
        void AddSignalHandler(WorldComponent::SignalHandlerConfig<Signal> config) {
            attachmentCore.AddSignalHandler<Signal>(config);
        }

        // MARK: SomeWorldComponent

        WorldNode* Node() const override {
            return owner;
        }

        SomeWorldComponent& SetNode(WorldNode* value) override {
            owner = value;
            return *this;
        }

        bool IsEnabled() const override;

        SomeWorldComponent& Enable(bool value) override;

        String Name() const override {
            return _core.name.size() > 0 ? _core.name : TypeName();
        }

        SomeWorldComponent& SetName(String value) override {
            _core.name = value;
            return *this;
        }

        String Id() const override {
            return _core.id;
        }

        SomeWorldComponent& SetId(String value) override {
            _core.id = value;
            return *this;
        }

        UP<UIPlan> MakeUIPlan(String context) override;

        SomeWorldComponent& AddSignalHandler(SignalHandler handler) override;

        void Signal(String id, SomeSignal const& signal) override {
            attachmentCore.Signal(id, signal);
        }

        FinishType OnUpdate(TimeSlice time) override {
            return attachmentCore.OnUpdate(time);
        }

        Updatable& GetUpdatable() override {
            return attachmentCore.updatable;
        }

        Updatables& GetUpdatables() override {
            return attachmentCore.updatables;
        }

        WorldNode* Target() override {
            GUARDR(targetFunc, owner)
            return targetFunc(*this);
        }

        bool IsAwake() const override {
            return life.IsAwake();
        }

        bool IsStarted() const override {
            return life.IsStarted();
        }

        void CheckedAwake() override {
            life.CheckedAwake([this]() { this->Awake(); });
        }

        void CheckedStart() override {
            life.CheckedStart([this]() { this->Start(); });
        }

        String TypeName() const override {
            return "WorldComponent";
        }

        virtual void LateUpdate() override {
            GUARD(lateUpdateFunc)
            lateUpdateFunc(*this);
        }

        void Awake() override {
            GUARD(awakeFunc)
            awakeFunc(*this);
        }

        void Start() override {
            GUARD(startFunc)
            startFunc(*this);
        }

        bool IsListener() const override {
            return !IsEmpty(attachmentCore.SignalHandlers());
        }

        bool IsListenerToSignal(String id) const override {
            return attachmentCore.IsListenerToSignal(id);
        }

    protected:
        /// Internal. Tracks the life cycle (Start, Awake) of this object
        WorldPartLife life;
    };
} // namespace PJ
