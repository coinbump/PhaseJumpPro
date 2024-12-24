#include "EmitFuncs.h"

using namespace std;
using namespace PJ;
using namespace EmitFuncs;

Emitter::EmitFunc EmitFuncs::Spread2D(Spread2DConfig config) {
    return [=](Emitter& emitter) {
        VectorList<EmitModel> result;

        auto count = config.count;

        GUARDR(count > 0, result)

        auto firstAngle = -config.angleStep.Degrees() * ((float)(count - 1) / 2.0f);
        auto random = emitter.random.get();

        for (int i = 0; i < count; i++) {
            EmitModel model;
            float angle = firstAngle + i * config.angleStep.Degrees();

            if (random) {
                angle += random->Delta(config.varyAngle.Degrees());
            }

            model.tags.Set(EmitModelTag::Angle, Angle::WithDegrees(angle));

            Add(result, model);
        }

        return result;
    };
}

Emitter::EmitFunc EmitFuncs::VaryAngle(VaryAngleConfig config) {
    return [=](Emitter& emitter) {
        VectorList<EmitModel> result;

        GUARDR(config.count > 0, result)
        auto random = emitter.random.get();

        for (int i = 0; i < config.count; i++) {
            EmitModel model;
            float emitAngle = random->VaryFloat(config.angle.Degrees(), config.varyAngle.Degrees());
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
