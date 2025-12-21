#pragma once

#include "LoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class GLTexture;

    /// Uses SDL to load a bitmap
    class SDLLoadBitmapOperation : public LoadResourcesOperation {
    public:
        using Base = LoadResourcesOperation;

        SDLLoadBitmapOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {
            loadResourcesFunc = [this](auto& operation) { return _LoadResources(); };
        }

        Result _LoadResources();
    };
} // namespace PJ
