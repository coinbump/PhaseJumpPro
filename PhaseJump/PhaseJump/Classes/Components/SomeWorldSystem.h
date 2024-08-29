#pragma once

#include "Base.h"
#include "List.h"
#include "Updatable.h"
#include "VectorList.h"
#include "WorldPartLife.h"
#include <memory>

/*
 RATING: 5 stars
TODO: needs unit tests
 CODE REVIEW: 7/14/24
 */
namespace PJ {
    class SomeUIEvent;
    class World;
    class RenderContextModel;

    struct RenderResult {
        /// Stores information from the render system used for logging and dev views
        Tags tags;
    };

    /// Exists outside of the world node graph and receives UI events
    class SomeWorldSystem : public Base, public Updatable {
    protected:
        World* world;

        WorldPartLife life;

        virtual void Awake() {}

        virtual void Start() {}

    public:
        /// Tags that define the object's type
        TypeTagSet typeTags;

        virtual ~SomeWorldSystem() {}

        World* World() const {
            return world;
        }

        void SetWorld(class World* value) {
            this->world = value;
        }

        /// Called before Start
        virtual void CheckedAwake() {
            life.CheckedAwake([this]() { Awake(); });
        }

        /// Called after Awake
        virtual void CheckedStart() {
            life.CheckedStart([this]() { Start(); });
        }

        /// Called when UI events are processed
        virtual void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) {}

        /// Called to render next frame
        virtual std::optional<RenderResult> Render(RenderContextModel& contextModel) {
            return std::nullopt;
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {}
    };
} // namespace PJ
