#pragma once

#include "LoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// Use SDL to load an audio file
    class SDLLoadAudioStreamOperation : public LoadResourcesOperation {
    public:
        using Base = LoadResourcesOperation;

        SDLLoadAudioStreamOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {
            loadResourcesFunc = [this](auto& operation) { return _LoadResources(); };
        }

        Result _LoadResources();
    };
} // namespace PJ
