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

        struct Spread2DConfig {
            int count = 1;
            Angle angleStep;
            Angle varyAngle;
        };

        struct VaryAngleConfig {
            int count = 1;
            Angle angle;
            Angle varyAngle;
        };

        /// Emits N objects in 2D space, spread by N degrees each, centered to 0
        Func Spread2D(Spread2DConfig config);

        /// Emits N objects in 2D space, with a varying emit angle
        Emitter::EmitFunc VaryAngle(VaryAngleConfig config);

        /// Modifies an emit func by adding varying directional velocity
        Emitter::EmitFunc AddVaryDirectionVelocity(
            Emitter::EmitFunc const& baseFunc, float velocity, float varyVelocity
        );
    }; // namespace EmitFuncs
} // namespace PJ
