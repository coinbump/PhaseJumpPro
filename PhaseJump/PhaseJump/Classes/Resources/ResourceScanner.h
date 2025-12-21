#pragma once

#include "FilePath.h"
#include "FileTypes.h"
#include "LoadResourcesOperation.h"
#include "OrderedMap.h"
#include "ResourceCatalog.h"
#include "ResourceRepositoryPlan.h"
#include "Result.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    class ResourceRepositoryModel;
    struct ResourceModel;
    struct ResourceInfo;
    class SomeFileManager;

    /// Interface to an object that scans for resources and creates a load
    /// resource plan
    class SomeResourceScanner {
    public:
        virtual ~SomeResourceScanner() {}

        virtual ResourceRepositoryPlan ScanAt(FilePath path, FileSearchType searchType) = 0;
    };

    /**
     A model for scanning resources for a resource repository

     Each file extension is mapped to a resource type
     */
    class ResourceScanModel {
    protected:
        /// Maps file extension to resource type
        UnorderedMap<String, String> fileExtensionMap;

    public:
        virtual ~ResourceScanModel() {}

        virtual std::optional<String> TypeForFileExtension(String fileExtension) {
            auto iterator = fileExtensionMap.find(fileExtension);
            GUARDR(iterator != fileExtensionMap.end(), std::nullopt)

            return iterator->second;
        }
    };

    /// Registers operations to load commonly used resource types
    class StandardResourceScanModel : public ResourceScanModel {
    public:
        enum class LoadType {
            /// Load .rez files with metadata that describes each resource
            Rez,

            /// Scan all files and load (won't work with complex resource types)
            Scan
        };

        StandardResourceScanModel(LoadType loadType = LoadType::Rez);
    };

    /// Scans for resources
    class ResourceScanner : public SomeResourceScanner {
    public:
        /// Defines how we're scanning/loading resources
        ResourceScanModel& scanModel;

        /// File manager (for mock injection in unit tests)
        SomeFileManager& fm;

        ResourceScanner(ResourceScanModel& scanModel, SomeFileManager& fm) :
            scanModel(scanModel),
            fm(fm) {}

        virtual std::optional<ResourceInfo> ScanFile(FilePath path);

        // MARK: SomeResourceScanner

        /**
         Important: always use this with threads or you will hang the app for
         large file lists
         */
        ResourceRepositoryPlan ScanAt(FilePath path, FileSearchType searchType) override;
    };
} // namespace PJ
