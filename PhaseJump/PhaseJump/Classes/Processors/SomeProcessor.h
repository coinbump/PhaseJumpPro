#pragma once

#include "Types.h"
#include <algorithm>

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 9/19/24
 */
namespace PJ {
    /// Takes an input and processes it to create an output over time
    template <class Input>
    class SomeProcessor {
    protected:
        Input input;

    public:
        SomeProcessor() {}

        virtual ~SomeProcessor() {}

        float Progress() const {
            return std::clamp((float)ProcessedCount() / (float)InputCount(), 0.0f, 1.0f);
        }

        virtual size_t InputCount() const = 0;
        virtual size_t ProcessedCount() const = 0;
    };
} // namespace PJ
