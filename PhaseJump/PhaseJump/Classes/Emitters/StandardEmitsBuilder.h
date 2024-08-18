#pragma once

#include "Emitter.h"

namespace PJ {
    /// Common methods of building emits for an emitter
    namespace StandardEmitsBuilder {
        /// Emit N objects, spread by N degrees each, centered to 0
        Emitter::BuildEmitsFunc MakeSpread2DFunc(int emitCount, Angle angleStep, Angle varyAngle);
    }; // namespace StandardEmitsBuilder
} // namespace PJ
