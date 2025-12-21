#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/4/24
 */
namespace PJ {
    class SomeRandom;

    /// Produces an output based an input
    /// Inherit this protocol to provide objects to another object that requests it
    template <class Output, class Input>
    class SomeProducer : public WorldComponent {
    public:
        virtual Output Produce(Input& input) = 0;
    };
} // namespace PJ
