#ifndef PJRESOURCEREPOSITORY_H
#define PJRESOURCEREPOSITORY_H

#include "_String.h"
#include "FilePath.h"
#include "LoadedResources.h"
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
        SP<LoadedResources> loadedResources;

        /// File manager (for mock injection in unit tests)
        SP<SomeFileManager> fm;

        ResourceRepository(
            SP<LoadResourcesModel> loadResourcesModel, SP<LoadedResources> loadedResources,
            SP<SomeFileManager> fm
        ) :
            loadResourcesModel(loadResourcesModel),
            loadedResources(loadedResources),
            fm(fm) {}

        void Load(LoadResourceInfo info);
        List<SP<SomeLoadResourcesOperation>> LoadOperations(LoadResourceInfo info);
        List<SP<SomeLoadResourcesOperation>> LoadOperations(LoadResourcesPlan plan);
    };
} // namespace PJ

#endif
