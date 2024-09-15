#pragma once

#include "SomeSignal.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 7/5/23
 */
namespace PJ {
    /// Receives events from broadcasters
    class SomeListener {
    public:
        virtual ~SomeListener() {}

        // TODO: why is this a pointer? Why not a reference?
        virtual void OnEvent(SP<SomeSignal> event) = 0;
    };
} // namespace PJ
