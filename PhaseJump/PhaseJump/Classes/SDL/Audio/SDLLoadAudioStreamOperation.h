#pragma once

#include "SomeLoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// Use SDL to load an audio file
    class SDLLoadAudioStreamOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        SDLLoadAudioStreamOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
