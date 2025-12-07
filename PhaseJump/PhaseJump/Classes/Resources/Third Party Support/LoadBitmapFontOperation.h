#pragma once

#include "SomeLoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// Load a texture atlas .fnt file created by Bitmap Font Generator
    class LoadBitmapFontOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        LoadBitmapFontOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
