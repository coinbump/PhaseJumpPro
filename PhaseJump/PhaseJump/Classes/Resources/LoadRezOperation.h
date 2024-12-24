#pragma once

#include "SomeLoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    /// Loads resource from a resource info file
    /// Allows us to specify what resources we need instead of loading everything
    class LoadRezOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        LoadRezOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
