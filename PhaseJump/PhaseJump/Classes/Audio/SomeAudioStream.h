#pragma once

#include "Base.h"
#include "Playable.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// Source for playable audio
    class SomeAudioStream : public Base, public SomePlayable {
    public:
        virtual ~SomeAudioStream() {}
    };
} // namespace PJ
