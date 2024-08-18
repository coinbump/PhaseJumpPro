#include "StandardEmitsBuilder.h"

using namespace std;
using namespace PJ;
using namespace StandardEmitsBuilder;

Emitter::BuildEmitsFunc
StandardEmitsBuilder::MakeSpread2DFunc(int emitCount, Angle angleStep, Angle varyAngle) {
    return [=](Emitter& emitter) {
        VectorList<EmitModel> result;

        GUARDR(emitCount > 0, result)

        auto firstAngle = -angleStep.Degrees() * ((float)(emitCount - 1) / 2.0f);

        for (int i = 0; i < emitCount; i++) {
            EmitModel model;
            float angle = firstAngle + i * angleStep.Degrees();

            auto random = emitter.random.get();
            if (random) {
                angle += random->Delta(varyAngle.Degrees());
            }

            model.tags.Add(EmitModelTag::Angle2D, Angle::DegreesAngle(angle));

            result.Add(model);
        }

        return result;
    };
}
