#pragma once

#include "LoadResourcesOperation.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    /// Loads resource from a resource info file
    /// Allows us to specify what resources we need instead of loading everything
    class LoadRezOperation : public LoadResourcesOperation {
    public:
        using Base = LoadResourcesOperation;

        LoadRezOperation(ResourceInfo info, ResourceRepositoryModel& repoModel) :
            Base(info, repoModel) {
            loadResourcesFunc = [this](auto& operation) { return _LoadResources(); };
        }

        Result _LoadResources();
    };
} // namespace PJ
