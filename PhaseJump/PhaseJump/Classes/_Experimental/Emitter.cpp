#include "Emitter.h"

using namespace std;
using namespace PJ;

// #define PROFILE

float Emitter::EmitTime() const {
    return fireTimer.duration;
}

void Emitter::SetEmitTime(float value) {
    fireTimer.SetDuration(value);
}

Emitter::Emitter(SpawnFunc spawnFunc, float fireTime) :
    spawnFunc(spawnFunc),
    fireTimer(fireTime, Runner::RunType::Repeat) {

    fireTimer.onFinishFunc =
        std::make_unique<Function<void(TimedPlayable&)>>([this](TimedPlayable& timedPlayable) {
            this->OnFireTimerFinish();
        });
}

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

    GUARD(EmitTime() > 0)
    fireTimer.OnUpdate(time);
}

bool Emitter::CanSpawn() {
    GUARDR(maxAlive > 0, true)

    emits = Refreshed(emits);
    return emits.size() < maxAlive;
}

Emitter::EmitList Emitter::Fire() {
    EmitList result;
    GUARDR(buildEmitsFunc, result)

    result = buildEmitsFunc(*this);
    EmitWithEmits(result);
    OnEmit(result);

    return result;
}

List<Emitter::SpawnType> Emitter::EmitWithEmits(EmitList emits) {
    List<Emitter::SpawnType> result;

    for (auto& emit : emits) {
        auto spawnDelay = emit.SpawnDelay();
        if (spawnDelay && *spawnDelay > 0) {
            Add(delayedEmits, DelayedEmitModel(emit, *spawnDelay));
            continue;
        }

        auto spawn = Spawn(emit);
        GUARD_CONTINUE(spawn)

        Add(result, spawn);
    }

    return result;
}

Emitter::SpawnType Emitter::MakeSpawn(EmitModel const& emit) {
    GUARDR(spawnFunc, nullptr)

    auto spawn = spawnFunc(*this, emit);
    GUARDR(spawn, nullptr)

    return *spawn;
}

Emitter::SpawnType Emitter::Spawn(EmitModel const& emit) {
    GUARDR(CanSpawn(), nullptr)

    SpawnType newSpawn = MakeSpawn(emit);
    GUARDR(newSpawn, nullptr)

    auto spawnPosition = SpawnPosition(newSpawn);

    WorldNode* spawnParent = !this->spawnParent.expired() ? this->spawnParent.lock().get() : owner;
    GUARDR(spawnParent, nullptr)

    spawnParent->Add(newSpawn);
    newSpawn->transform.SetLocalPosition(spawnPosition);

    if (maxAlive > 0) {
        emits.insert(newSpawn);
    }

    OnSpawn(newSpawn);

    if (onSpawnFunc) {
        onSpawnFunc(newSpawn);
    }

    return newSpawn;
}

void Emitter::OnFireTimerFinish() {
    Fire();
}

/// Returns local position for new spawn
Vector3 Emitter::SpawnPosition(SpawnType spawn) {
    GUARDR(positionFunc, Vector3::zero)
    return positionFunc(*random);
}
