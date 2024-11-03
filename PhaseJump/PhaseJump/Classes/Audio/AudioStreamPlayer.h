#pragma once

#include "SomeAudioStream.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/9/24
 */
namespace PJ {
    // TODO: should we use composition or inheritance for components?
    class AudioStreamPlayer {
    public:
        SP<SomeAudioStream> audioStream;

        void Play() {
            GUARD(audioStream)
            audioStream->Play();
        }

        // MARK: SomeWorldComponent

        String TypeName() const {
            return "AudioStreamPlayer";
        }
    };
} // namespace PJ
