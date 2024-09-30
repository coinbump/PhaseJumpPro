#pragma once

#include "Base.h"
#include "Playable.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/17/24
 */
namespace PJ {
    // TODO: re-evaluate use of multiple inheritance here
    /// An audio stream is a source for playable audio
    class SomeAudioStream : public Base, public Playable {
    public:
        virtual ~SomeAudioStream() {}
    };
} // namespace PJ
