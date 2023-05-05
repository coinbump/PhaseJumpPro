#ifndef PJSOMELOADRESOURCESOPERATION_H
#define PJSOMELOADRESOURCESOPERATION_H

#include "SomeOperation.h"
#include "Tags.h"
#include "FilePath.h"
#include "LoadedResources.h"
#include "LoadResourceInfo.h"
#include "Result.h"
#include "_Map.h"

/*
 RATING: 5 stars
 Utility type
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    /// Loads N resources with arguments and places it in result
    class SomeLoadResourcesOperation : public SomeOperation {
    public:
        using Success = VectorList<LoadedResource>;
        using Failure = std::exception;
        using Result = PJ::Result<Success, Failure>;

        // Input:
        LoadResourceInfo info;
        Tags arguments;

        // Output:
        SP<Result> result;

        // Required for Factory registry if type is not available (this class can't be abstract)
        void Run() override { }
    };
}

#endif
