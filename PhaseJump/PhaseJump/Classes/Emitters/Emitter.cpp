#include "Emitter.h"
// #include <box2d/box2d.h>

using namespace std;
using namespace PJ;

// #define PROFILE

// void foo() {
//     b2WorldDef worldDef = b2DefaultWorldDef();
//     //    b2WorldDef worldDef2 = b2DefaultWorldDef();
//     //    worldDef.workerCount = settings.workerCount;
//     //    worldDef.enqueueTask = EnqueueTask;
//     //    worldDef.finishTask = FinishTask;
//     //    worldDef.userTaskContext = this;
//     //    worldDef.enableSleep = settings.enableSleep;
//
//     auto m_worldId = b2CreateWorld(&worldDef);
// }

using SpawnList = PJ::Emitter::SpawnList;

Emitter::Emitter() {}

Emitter::Emitter(SpawnFunc spawnFunc, float fireTime) :
    spawnFunc(spawnFunc) {
    driver = NEW<TimerDriver>(fireTime, RunType::Repeat, [this]() { this->Fire(); });
}

Emitter::Emitter(SpawnFunc spawnFunc, UP<SomeDriver>& fireDriver) :
    spawnFunc(spawnFunc),
    driver(std::move(fireDriver)) {}

void Emitter::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    EmitList finishedEmits;
    List<DelayedEmitModel> unfinishedEmits;
    for (auto& delayedEmit : delayedEmits) {
        delayedEmit.OnUpdate(time);

        if (delayedEmit.delay <= 0) {
            finishedEmits.push_back(delayedEmit.model);
        } else {
            unfinishedEmits.push_back(delayedEmit);
        }
    }
    delayedEmits = unfinishedEmits;
    EmitWithEmits(finishedEmits);

    GUARD(driver);
    driver->OnUpdate(time);
}

bool Emitter::CanSpawn() {
    GUARDR(maxAlive > 0, true)

    emits = Refreshed(emits);
    return emits.size() < maxAlive;
}

Emitter::EmitList Emitter::Fire() {
    EmitList result;
    GUARDR(emitFunc, result)

    result = emitFunc(*this);
    EmitWithEmits(result);
    OnEmit(result);

    return result;
}

List<Emitter::SpawnType> Emitter::EmitWithEmits(EmitList emits) {
    List<Emitter::SpawnType> result;

    for (auto& emit : emits) {
        auto spawnDelay = emit.Delay();
        if (spawnDelay && *spawnDelay > 0) {
            Add(delayedEmits, DelayedEmitModel(emit, *spawnDelay));
            continue;
        }

        auto spawns = Spawn(emit);
        for (auto& spawn : spawns) {
            Add(result, spawn);
        }
    }

    return result;
}

SpawnList Emitter::MakeSpawns(EmitModel const& emit) {
    GUARDR(spawnFunc, {})
    return spawnFunc(*this, emit);
}

SpawnList Emitter::Spawn(EmitModel const& emit) {
    GUARDR(CanSpawn(), {})

    auto spawns = MakeSpawns(emit);
    for (auto& newSpawn : spawns) {
        auto spawnPosition = SpawnPosition(newSpawn);

        WorldNode* spawnParent =
            !this->spawnParent.expired() ? this->spawnParent.lock().get() : owner;
        GUARDR(spawnParent, {})

        spawnParent->Add(newSpawn);
        newSpawn->transform.SetLocalPosition(spawnPosition);

        if (maxAlive > 0) {
            emits.insert(newSpawn);
        }

        OnSpawn(newSpawn);
    }

    return spawns;
}

/// @return Returns local position for new spawn
Vector3 Emitter::SpawnPosition(SpawnType spawn) {
    GUARDR(positionFunc, Vector3::zero)
    return positionFunc(*this);
}
