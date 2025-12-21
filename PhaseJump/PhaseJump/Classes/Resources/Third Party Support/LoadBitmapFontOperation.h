#pragma once

#include "LoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// Load a texture atlas .fnt file created by Bitmap Font Generator
    class LoadBitmapFontOperation : public LoadResourcesOperation {
    public:
        using Base = LoadResourcesOperation;

        LoadBitmapFontOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {
            loadResourcesFunc = [this](auto& operation) { return _LoadResources(); };
        }

        Result _LoadResources();
    };
} // namespace PJ
