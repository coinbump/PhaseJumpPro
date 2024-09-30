#pragma once

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

namespace Example {
    namespace Checkers {
        /// Renders the 8x8 checkboard with a mesh
        class MatrixRenderer : public SomeRenderer {
        public:
            using Base = SomeRenderer;

        public:
            MatrixRenderer(Vector3 worldSize);

            // MARK: SomeWorldComponent

            String TypeName() const override {
                return "ExampleCheckers-MatrixRenderer";
            }
        };
    } // namespace Checkers
} // namespace Example
