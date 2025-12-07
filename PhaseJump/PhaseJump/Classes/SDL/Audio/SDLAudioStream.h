#pragma once

#include "Playable.h"
#include "SomeAudioStream.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

/*
 RATING: 4 stars
 Tested and works, but missing functionality
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// An audio stream plays a single stream of audio
    class SDLAudioStream : public SomeAudioStream {
    protected:
        Uint8* audioBuffer{};
        Uint32 bufferLength{};
        SDL_AudioSpec audioSpec;
        SDL_AudioStream* stream{};

    public:
        SDLAudioStream(Uint8* audioBuffer, Uint32 bufferLength, SDL_AudioSpec audioSpec);

        virtual ~SDLAudioStream();

        void Flush();

        // MARK: SomeUpdatable

        virtual FinishType OnUpdate(TimeSlice time) override {
            return FinishType::Continue;
        }

        /// Allows for cleanup of old updatables
        bool IsFinished() const override {
            return false;
        }

        // MARK: SomePlayable

        void Play() override;

        // FUTURE: support missing playable methods
        float Duration() const override {
            return 0;
        }

        void SetDuration(float value) override {
            PJ::Log("ERROR. Can't set duration of audio stream");
        }

        float Progress() const override {
            return 0;
        }

        void SetProgress(float value) override {}

        void Pause() override {
            GUARD(stream)
            SDL_PauseAudioDevice(SDL_GetAudioStreamDevice(stream));
        }

        void Stop() override;

        float PlayTime() const override {
            return 0;
        }

        void SetPlayTime(float time) override {}

        bool IsPaused() const override {
            GUARDR(stream, false)
            return SDL_AudioDevicePaused(SDL_GetAudioStreamDevice(stream));
        }

        /// @return Returns true if the playable is playing
        bool IsPlaying() const override {
            return !IsPaused();
        }
    };
} // namespace PJ
