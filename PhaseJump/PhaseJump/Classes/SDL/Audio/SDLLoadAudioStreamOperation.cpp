#include "SDLLoadAudioStreamOperation.h"
#include "SDLAudioStream.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

using namespace std;
using namespace PJ;

SomeLoadResourcesOperation::Result SDLLoadAudioStreamOperation::LoadResources() {
    Uint8* audioBuffer = nullptr;
    Uint32 bufferLength = 0;
    SDL_AudioSpec inputAudioSpec;

    // https://wiki.libsdl.org/SDL3/Tutorials/AudioStream
    if (SDL_LoadWAV(info.filePath.c_str(), &inputAudioSpec, &audioBuffer, &bufferLength) == 0) {
        auto audioStream = MAKE<SDLAudioStream>(audioBuffer, bufferLength, inputAudioSpec);

        ResourceModel loadedResource(
            SCAST<PJ::Base>(audioStream), info.id, info.filePath, info.type
        );
        Success result;
        result.Add(loadedResource);
        return result;
    }

    return Failure();
}
