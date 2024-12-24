#pragma once

#include "FilePath.h"
#include "OrderedMap.h"
#include "ResourceCatalog.h"
#include "ResourceRepositoryPlan.h"
#include "Result.h"
#include "SomeLoadResourcesOperation.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    class ResourceRepositoryModel;
    struct ResourceInfo;
    class SomeFileManager;

    /// Interface to an object that loads resources
    class SomeResourceRepository {
    public:
        virtual ~SomeResourceRepository() {}

        virtual VectorList<SP<SomeLoadResourcesOperation>> MakeLoadOperations(ResourceInfo info
        ) = 0;
        virtual VectorList<SP<SomeLoadResourcesOperation>>
        MakeLoadOperations(ResourceRepositoryPlan plan) = 0;
    };

    /// Loads resources
    class ResourceRepository : public SomeResourceRepository {
    protected:
        void Run(VectorList<SP<SomeLoadResourcesOperation>> const& operations);

    public:
        /// Contains registered factories to create load resource operations
        ResourceRepositoryModel& repoModel;

        /// Destination for the loaded resources
        ResourceCatalog& resourceCatalog;

        /// File manager (for mock injection in unit tests)
        SomeFileManager& fm;

        ResourceRepository(
            ResourceRepositoryModel& repoModel, ResourceCatalog& resourceCatalog,
            SomeFileManager& fm
        ) :
            repoModel(repoModel),
            resourceCatalog(resourceCatalog),
            fm(fm) {}

        void Load(ResourceInfo info);

        // MARK: SomeResourceRepository

        VectorList<SP<SomeLoadResourcesOperation>> MakeLoadOperations(ResourceInfo info) override;
        VectorList<SP<SomeLoadResourcesOperation>> MakeLoadOperations(ResourceRepositoryPlan plan
        ) override;
    };
} // namespace PJ
