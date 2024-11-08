#pragma once

#include "SomeLoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Load a texture atlas JSON created by TexturePacker
    class LoadTexturePackerAtlasOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        LoadTexturePackerAtlasOperation(ResourceInfo info, LoadResourcesModel& loadResourcesModel) :
            Base(info, loadResourcesModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
