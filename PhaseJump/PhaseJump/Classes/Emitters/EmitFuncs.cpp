#include "EmitFuncs.h"

using namespace std;
using namespace PJ;
using namespace EmitFuncs;

Emitter::EmitFunc EmitFuncs::MakeSpread2D(int count, Angle angleStep, Angle varyAngle) {
    return [=](Emitter& emitter) {
        VectorList<EmitModel> result;

        GUARDR(count > 0, result)

        auto firstAngle = -angleStep.Degrees() * ((float)(count - 1) / 2.0f);
        auto random = emitter.random.get();

        for (int i = 0; i < count; i++) {
            EmitModel model;
            float angle = firstAngle + i * angleStep.Degrees();

            if (random) {
                angle += random->Delta(varyAngle.Degrees());
            }

            model.tags.Set(EmitModelTag::Angle, Angle::WithDegrees(angle));

            Add(result, model);
        }

        return result;
    };
}

Emitter::EmitFunc EmitFuncs::MakeVaryAngle(int count, Angle angle, Angle varyAngle) {
    return [=](Emitter& emitter) {
        VectorList<EmitModel> result;

        GUARDR(count > 0, result)
        auto random = emitter.random.get();

        for (int i = 0; i < count; i++) {
            EmitModel model;
            float emitAngle = random->VaryFloat(angle.Degrees(), varyAngle.Degrees());
            model.tags.Set(EmitModelTag::Angle, Angle::WithDegrees(emitAngle));

            Add(result, model);
        }

        return result;
    };
}

Emitter::EmitFunc EmitFuncs::AddVaryDirectionVelocity(
    Emitter::EmitFunc const& baseFunc, float velocity, float varyVelocity
) {
    return [=](Emitter& emitter) {
        auto random = emitter.random.get();

        auto result = baseFunc(emitter);
        for (auto& model : result) {
            float emitVelocity = random->VaryFloat(velocity, varyVelocity);
            model.tags.Set(EmitModelTag::StartDirectionVelocity, emitVelocity);
        }

        return result;
    };
}
