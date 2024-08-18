#pragma once

#include "_String.h"
#include "FilePath.h"
#include "LoadResourcesPlan.h"
#include "OrderedMap.h"
#include "Result.h"
#include "SomeLoadResourcesOperation.h"

// CODE REVIEW: ?/23
namespace PJ {
    class LoadResourcesModel;
    struct LoadedResource;
    struct LoadResourceInfo;
    class SomeFileManager;
    struct LoadedResources;

    /// Interface to an object that scans for resources and creates a load
    /// resource plan
    class SomeResourceScanner {
    public:
        virtual ~SomeResourceScanner() {}

        virtual LoadResourcesPlan ScanAt(FilePath path, bool isRecursive) = 0;
    };

    class ResourceScanner : public SomeResourceScanner {
    public:
        /// Defines how we're scanning/loading resources
        SP<LoadResourcesModel> loadResourcesModel;

        /// File manager (for mock injection in unit tests)
        SP<SomeFileManager> fm;

        ResourceScanner(SP<LoadResourcesModel> loadResourcesModel, SP<SomeFileManager> fm) :
            loadResourcesModel(loadResourcesModel),
            fm(fm) {}

        // Important: always use this with threads or you will hang the app for
        // large file lists
        LoadResourcesPlan ScanAt(FilePath path, bool isRecursive) override;

        virtual std::optional<LoadResourceInfo> ScanFile(FilePath path);
    };
} // namespace PJ
