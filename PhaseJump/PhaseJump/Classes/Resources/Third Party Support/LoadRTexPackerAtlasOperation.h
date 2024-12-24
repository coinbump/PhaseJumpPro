#pragma once

#include "SomeLoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Load a texture atlas JSON created by rTexPacker
    class LoadRTexPackerAtlasOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        LoadRTexPackerAtlasOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
