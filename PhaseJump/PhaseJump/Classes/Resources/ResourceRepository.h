#ifndef PJRESOURCEREPOSITORY_H
#define PJRESOURCEREPOSITORY_H

#include "FilePath.h"
#include "Result.h"
#include "LoadResourcesPlan.h"
#include "SomeLoadResourcesOperation.h"
#include "_Map.h"
#include "_String.h"

namespace PJ {
    class SomeLoadResourcesModel;
    struct LoadedResource;
    struct LoadResourceInfo;
    class SomeFileManager;
    struct LoadedResources;

    /// Interface to an object that loads resources
    class SomeResourceRepository {
    public:
        virtual ~SomeResourceRepository() {}

        virtual LoadResourcesPlan Scan(FilePath path, bool isRecursive) = 0;
    };

    /// Scans and loads resources
    class ResourceRepository : public SomeResourceRepository {
    public:
        /// Defines how we're scanning/loading resources
        SP<SomeLoadResourcesModel> loadResourcesModel;

        /// Destination for the loaded resources
        SP<LoadedResources> loadedResources;

        /// File manager (for mock injection in unit tests)
        SP<SomeFileManager> fm;

        ResourceRepository(SP<SomeLoadResourcesModel> loadResourcesModel, SP<LoadedResources> loadedResources, SP<SomeFileManager> fm) :
        loadResourcesModel(loadResourcesModel),
        loadedResources(loadedResources),
        fm(fm) {
        }

        // Important: always use this with threads or you will hang the app for large file lists
        LoadResourcesPlan Scan(FilePath path, bool isRecursive) override;
        void Load(LoadResourceInfo info);
        SP<SomeLoadResourcesOperation> LoadOperation(LoadResourceInfo info);
        VectorList<SP<SomeLoadResourcesOperation>> LoadOperations(LoadResourcesPlan plan);
        
    protected:
        std::optional<LoadResourceInfo> ScanFile(FilePath path);
    };
}

#endif
