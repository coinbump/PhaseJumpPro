#pragma once

#include "SomeRandom.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Returns a fixed set of random values (for testing, debugging)
    class FixedRandom : public SomeRandom {
    public:
        using T = float;
        using Allocator = VectorList<float>::allocator_type;

        VectorList<float> values;

    protected:
        int valueIndex = 0;

    public:
        FixedRandom() {}

        constexpr FixedRandom(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
            values(init, alloc) {}

        float Value() override {
            if (values.size() == 0) {
                return 0;
            }

            auto index = valueIndex % values.size();
            valueIndex++;
            return values[index];
        }
    };
} // namespace PJ
