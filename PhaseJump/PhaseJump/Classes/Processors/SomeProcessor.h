#pragma once

#include "Types.h"
#include <algorithm>

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    /// Takes an input and processes it to create an output over time
    class SomeProcessor {
    public:
        SomeProcessor() {}

        virtual ~SomeProcessor() {}

        float Progress() const {
            return std::clamp((float)ProcessedCount() / (float)InputCount(), 0.0f, 1.0f);
        }

        virtual size_t InputCount() const = 0;
        virtual size_t ProcessedCount() const = 0;
    };

    /// Takes an input and processes it to create an output over time
    template <class Input>
    class InputProcessor : public SomeProcessor {
    protected:
        Input input;

    public:
        InputProcessor() {}

        virtual ~InputProcessor() {}
    };
} // namespace PJ
