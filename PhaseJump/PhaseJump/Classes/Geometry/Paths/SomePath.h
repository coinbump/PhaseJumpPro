#pragma once

#include "Transformer.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// A path defines positions along the path from 0-1
    class SomePath : public SomeTransformer<float, Vector3> {
    public:
        virtual ~SomePath() {}

        virtual Vector3 PositionAt(float progress) = 0;

        // FUTURE: support PathLength if needed

        // MARK: SomeTransformer

        Vector3 Transform(float value) override {
            return PositionAt(value);
        }
    };
} // namespace PJ
