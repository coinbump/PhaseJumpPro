#pragma once

#include "FilePath.h"
#include "FileTypes.h"
#include "LoadResourcesPlan.h"
#include "OrderedMap.h"
#include "Result.h"
#include "SomeLoadResourcesOperation.h"
#include "StringUtils.h"

// CODE REVIEW: ?/23
namespace PJ {
    class LoadResourcesModel;
    struct ResourceModel;
    struct ResourceInfo;
    class SomeFileManager;
    struct ResourceModels;

    /// Interface to an object that scans for resources and creates a load
    /// resource plan
    class SomeResourceScanner {
    public:
        virtual ~SomeResourceScanner() {}

        virtual LoadResourcesPlan ScanAt(FilePath path, FileSearchType searchType) = 0;
    };

    class ResourceScanner : public SomeResourceScanner {
    public:
        /// Defines how we're scanning/loading resources
        SP<LoadResourcesModel> loadResourcesModel;

        // TODO: why do we need SP here?
        /// File manager (for mock injection in unit tests)
        SP<SomeFileManager> fm;

        ResourceScanner(SP<LoadResourcesModel> loadResourcesModel, SP<SomeFileManager> fm) :
            loadResourcesModel(loadResourcesModel),
            fm(fm) {}

        // Important: always use this with threads or you will hang the app for
        // large file lists
        LoadResourcesPlan ScanAt(FilePath path, FileSearchType searchType) override;

        virtual std::optional<ResourceInfo> ScanFile(FilePath path);
    };
} // namespace PJ
