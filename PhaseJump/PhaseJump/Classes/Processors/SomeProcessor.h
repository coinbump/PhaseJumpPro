#ifndef PJSOMEPROCESSOR_H
#define PJSOMEPROCESSOR_H

#include "Types.h"
#include <algorithm>

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Takes an input and processes it to create an output over time
    template <class Input>
    class SomeProcessor {
    protected:
        Input input;
        Int inputCount = 0;
        Int processedInputCount = 0;

    public:
        SomeProcessor(Int inputCount = 0) :
            inputCount(inputCount) {}

        virtual ~SomeProcessor() {}

        virtual float Progress() const {
            return std::clamp((float)processedInputCount / (float)inputCount, 0.0f, 1.0f);
        }
    };
} // namespace PJ

#endif
