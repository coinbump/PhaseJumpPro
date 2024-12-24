#pragma once

#include "SDKIncludes.h"

namespace Example {
    namespace Checkers {
        /// Renders the 8x8 checkboard with a mesh
        class MatrixRenderer : public SomeMaterialRenderer {
        public:
            using Base = SomeMaterialRenderer;

        public:
            MatrixRenderer(Vector3 worldSize);

            // MARK: SomeWorldComponent

            String TypeName() const override {
                return "ExampleCheckers-MatrixRenderer";
            }
        };
    } // namespace Checkers
} // namespace Example
