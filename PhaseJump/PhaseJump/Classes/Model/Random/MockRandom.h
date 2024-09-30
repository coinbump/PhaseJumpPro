#pragma once

#include "Macros.h"
#include "SomeRandom.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/19/24
 */
namespace PJ {
    /// @return Returns a fixed set of random values (for testing, debugging)
    class MockRandom : public SomeRandom {
    public:
        using T = float;
        using Allocator = VectorList<float>::allocator_type;

        VectorList<float> values;

    protected:
        int valueIndex = 0;

    public:
        MockRandom() {}

        constexpr MockRandom(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
            values(init, alloc) {}

        float Value() override {
            GUARDR(!IsEmpty(values), 0)

            auto index = valueIndex % values.size();
            valueIndex++;
            return values[index];
        }

        int IntValue(int start, int end) override {
            // Not supported
            return 0;
        }
    };
} // namespace PJ
