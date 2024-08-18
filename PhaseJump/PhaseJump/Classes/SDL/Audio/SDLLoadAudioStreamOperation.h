#pragma once

#include "FilePath.h"
#include "SomeLoadResourcesOperation.h"
#include "SomeOperation.h"

// CODE REVIEW: ?/23
namespace PJ {
    /// Use SDL to load an OpenGL texture
    class SDLLoadAudioStreamOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        SDLLoadAudioStreamOperation(LoadResourceInfo info, LoadResourcesModel& loadResourcesModel) :
            Base(info, loadResourcesModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
