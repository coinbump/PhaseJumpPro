#pragma once

#include "SomeAudioStream.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    class AudioStreamPlayer : public WorldComponent<> {
    public:
        SP<SomeAudioStream> audioStream;

        void Play() {
            GUARD(audioStream)
            audioStream->Play();
        }
    };
} // namespace PJ
