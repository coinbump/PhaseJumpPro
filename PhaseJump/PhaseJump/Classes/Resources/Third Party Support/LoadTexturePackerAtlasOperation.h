#pragma once

#include "LoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Load a texture atlas JSON created by TexturePacker
    class LoadTexturePackerAtlasOperation : public LoadResourcesOperation {
    public:
        using Base = LoadResourcesOperation;

        LoadTexturePackerAtlasOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {
            loadResourcesFunc = [this](auto& operation) { return _LoadResources(); };
        }

        Result _LoadResources();
    };
} // namespace PJ
