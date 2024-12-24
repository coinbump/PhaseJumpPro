#include "SDLAudioStream.h"

using namespace std;
using namespace PJ;

SDLAudioStream::SDLAudioStream(Uint8* audioBuffer, Uint32 bufferLength, SDL_AudioSpec audioSpec) :
    audioBuffer(audioBuffer),
    bufferLength(bufferLength),
    audioSpec(audioSpec) {}

SDLAudioStream::~SDLAudioStream() {
    Flush();
}

void SDLAudioStream::Flush() {
    Stop();

    if (audioBuffer) {
        SDL_free(audioBuffer);
    }

    audioBuffer = {};
}

void SDLAudioStream::Stop() {
    GUARD(stream)
    SDL_DestroyAudioStream(stream);
}

void SDLAudioStream::Play() {
    if (IsPaused()) {
        SDL_ResumeAudioStreamDevice(stream);
        return;
    }

    GUARD(audioBuffer)
    GUARD(bufferLength > 0)

    // Stop previous instance of this audio or we'll end up playing this stream after the first
    // stream data finishes
    Stop();

    stream =
        SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec, nullptr, nullptr);

    SDL_PutAudioStreamData(stream, audioBuffer, bufferLength);
    SDL_FlushAudioStream(stream);
    SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(stream));
}
