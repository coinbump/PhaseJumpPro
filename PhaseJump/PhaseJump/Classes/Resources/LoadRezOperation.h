#pragma once

#include "SomeLoadResourcesOperation.h"

// CODE REVIEW: ?/23
namespace PJ {
    /// Loads resource from a resource info file
    /// Allows us to specify what resources we need instead of loading everything
    class LoadRezOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        LoadRezOperation(LoadResourceInfo info, LoadResourcesModel& loadResourcesModel) :
            Base(info, loadResourcesModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
