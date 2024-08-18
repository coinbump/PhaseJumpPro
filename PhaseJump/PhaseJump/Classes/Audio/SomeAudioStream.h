#pragma once

#include "Base.h"
#include "Playable.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/17/24
 */
namespace PJ {
    /// An audio stream is a source for playable audio
    class SomeAudioStream : public Base, public Playable {
    public:
        virtual ~SomeAudioStream() {}
    };
} // namespace PJ
