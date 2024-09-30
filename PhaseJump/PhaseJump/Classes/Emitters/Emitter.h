#pragma once

#include "Dev.h"
#include "EmitModel.h"
#include "EmitterTypes.h"
#include "OrderedSet.h"
#include "SomeDriver.h"
#include "StandardRandom.h"
#include "Timer.h"
#include "WorldComponent.h"
#include "WorldNode.h"
#include <memory>

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 8/16/24
 */
namespace PJ {
    // TODO: needs unit tests
    /// Interface to an emitter. For the emitter-spawner pattern
    /// Each emitter uses a spawner to spawn new objects that it emits
    class SomeEmitter {
    public:
        virtual ~SomeEmitter() {}
    };

    /// Emitter spawns objects at intervals
    class Emitter : public WorldComponent<>, public SomeEmitter {
    public:
        using Base = WorldComponent<>;
        using This = Emitter;
        using SpawnType = SP<WorldNode>;

        using EmitList = VectorList<EmitModel>;
        using BuildEmitsFunc = std::function<EmitList(This& emitter)>;
        using SpawnFunc = std::function<std::optional<SpawnType>(This& emitter, EmitModel emit)>;
        using OnSpawnFunc = std::function<void(SpawnType)>;

        /// (Optional) Parent object to spawn new objects into
        /// If this is null, spawned objects are created as children of the emitter's world node
        WP<WorldNode> spawnParent;

        /// (Optional) Max # of emit objects alive (0 is ignored)
        int maxAlive = 0;

        /// Sent when an object is spawned
        OnSpawnFunc onSpawnFunc;

        // FUTURE: support resolver if needed
        SP<SomeRandom> random = MAKE<StandardRandom>();

        /// Builds emit models for emitted objects
        BuildEmitsFunc buildEmitsFunc = [](This& emitter) {
            EmitList result;
            result.push_back(EmitModel());
            return result;
        };

        /// Spawns objects based on emit models
        SpawnFunc spawnFunc = [](This& emitter, EmitModel emit) {
            PJ::Log("ERROR. Must implement a spawn func");
            return std::nullopt;
        };

        /// Determines position for spawned object
        PositionFunc positionFunc = [](SomeRandom& random) { return Vector3::zero; };

    protected:
        using WeakNodeSet = WeakOrderedSet<WP<WorldNode>>;

        /// Keep track of alive emits to enforce settings like max # of alive
        WeakNodeSet emits;

        /// Emits that have been built, but are delayed
        List<DelayedEmitModel> delayedEmits;

        /// Driver for building emits
        UP<SomeDriver> fireDriver;

    public:
        Emitter(SpawnFunc spawnFunc, float fireTime);
        Emitter(SpawnFunc spawnFunc, UP<SomeDriver>& fireDriver);

        virtual bool CanSpawn();

        virtual EmitList Fire();

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "Emitter";
        }

    protected:
        virtual List<SpawnType> EmitWithEmits(EmitList emits);

        virtual SpawnType MakeSpawn(EmitModel const& emit);

        virtual SpawnType Spawn(EmitModel const& emit);

        /// @return Returns local position for new spawn
        virtual Vector3 SpawnPosition(SpawnType spawn);

        /// Called when an object is spawned
        virtual void OnSpawn(SpawnType spawn) {}

        /// Called when objects are emitted
        virtual void OnEmit(EmitList const& spawns) {}

    public:
        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;
    };
} // namespace PJ
