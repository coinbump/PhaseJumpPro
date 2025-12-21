#include "SDLLoadAudioStreamOperation.h"
#include "SDLAudioStream.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

using namespace std;
using namespace PJ;

LoadResourcesOperation::Result SDLLoadAudioStreamOperation::_LoadResources() {
    SDL_AudioSpec inputAudioSpec;
    Uint32 bufferLength{};
    Uint8* audioBuffer{};

    bool isAudioLoaded =
        SDL_LoadWAV(info.filePath.c_str(), &inputAudioSpec, &audioBuffer, &bufferLength);
    GUARDR(isAudioLoaded, Failure())

    auto audioStream = MAKE<SDLAudioStream>(audioBuffer, bufferLength, inputAudioSpec);

    ResourceModel loadedResource{ .resource = audioStream, .info = info };
    Success result;
    result.Add(loadedResource);

    return result;
}
