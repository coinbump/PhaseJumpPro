#pragma once

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

    /// Exists outside of the world node graph and receives UI events
    class SomeWorldSystem : public Base, public Updatable {
    protected:
        World* world;

        virtual void Awake() {}

        virtual void Start() {}

    public:
        WorldPartLife life;

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

        virtual void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) {}

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {}
    };
} // namespace PJ
