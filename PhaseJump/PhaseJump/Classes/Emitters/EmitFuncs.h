#pragma once

#include "Emitter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 11/1/24
 */
namespace PJ {
    /// Common methods of building emits for an emitter
    namespace EmitFuncs {
        using Func = Emitter::EmitFunc;

        /// Emits N objects in 2D space, spread by N degrees each, centered to 0
        Func MakeSpread2D(int count, Angle angleStep, Angle varyAngle);

        /// Emits N objects in 2D space, with a varying emit angle
        Emitter::EmitFunc MakeVaryAngle(int count, Angle angle, Angle varyAngle);

        /// Modifies an emit func by adding varying directional velocity
        Emitter::EmitFunc AddVaryDirectionVelocity(
            Emitter::EmitFunc const& baseFunc, float velocity, float varyVelocity
        );
    }; // namespace EmitFuncs
} // namespace PJ
