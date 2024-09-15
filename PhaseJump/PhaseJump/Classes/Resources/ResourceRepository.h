#ifndef PJRESOURCEREPOSITORY_H
#define PJRESOURCEREPOSITORY_H

#include "FilePath.h"
#include "LoadResourcesPlan.h"
#include "OrderedMap.h"
#include "ResourceModels.h"
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

    /// Interface to an object that loads resources
    class SomeResourceRepository {
    public:
        virtual ~SomeResourceRepository() {}
    };

    /// Scans and loads resources
    class ResourceRepository : public SomeResourceRepository {
    protected:
        void Run(List<SP<SomeLoadResourcesOperation>> const& operations);

    public:
        // TODO: SP-audit
        /// Defines how we're scanning/loading resources
        SP<LoadResourcesModel> loadResourcesModel;

        /// Destination for the loaded resources
        SP<ResourceModels> loadedResources;

        /// File manager (for mock injection in unit tests)
        SP<SomeFileManager> fm;

        ResourceRepository(
            SP<LoadResourcesModel> loadResourcesModel, SP<ResourceModels> loadedResources,
            SP<SomeFileManager> fm
        ) :
            loadResourcesModel(loadResourcesModel),
            loadedResources(loadedResources),
            fm(fm) {}

        void Load(ResourceInfo info);
        List<SP<SomeLoadResourcesOperation>> LoadOperations(ResourceInfo info);
        List<SP<SomeLoadResourcesOperation>> LoadOperations(LoadResourcesPlan plan);
    };
} // namespace PJ

#endif
