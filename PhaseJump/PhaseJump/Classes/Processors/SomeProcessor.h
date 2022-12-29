#ifndef PJSOMEPROCESSOR_H
#define PJSOMEPROCESSOR_H

#include "Types.h"
#include <math.h>

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 11/12/22
 */
namespace PJ {
    /// <summary>
    /// Takes an input and processes it to create an output over time
    /// </summary>
    template <class Input> class SomeProcessor {
    protected:
        Input input;
        Int inputCount = 0;
        Int processedInputCount = 0;

    public:
        SomeProcessor(Int inputCount = 0) : inputCount(inputCount) {}
        virtual ~SomeProcessor() {}

        virtual float Progress() const {
            return fmin(1.0f, fmax(0.0f, (float)processedInputCount/(float)inputCount));
        }
    };
}

#endif
