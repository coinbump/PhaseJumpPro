#pragma once

#include "SomeLoadResourcesOperation.h"

/*
 RATING: 4 stars
 Tested and works, but relies on OpenGL load texture operation (for now)
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Load a texture atlas JSON created by TexturePacker
    class LoadTexturePackerAtlasOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        LoadTexturePackerAtlasOperation(
            LoadResourceInfo info, LoadResourcesModel& loadResourcesModel
        ) :
            Base(info, loadResourcesModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
