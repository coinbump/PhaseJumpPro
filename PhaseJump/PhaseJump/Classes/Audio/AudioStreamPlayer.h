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

        void Pause() {
            GUARD(audioStream)
            audioStream->Pause();
        }

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
