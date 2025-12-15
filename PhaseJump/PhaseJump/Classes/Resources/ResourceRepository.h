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
        using LoadResourcesOperationList = VectorList<SP<SomeLoadResourcesOperation>>;

        virtual ~SomeResourceRepository() {}

        virtual LoadResourcesOperationList MakeLoadOperations(ResourceInfo info) = 0;
        virtual LoadResourcesOperationList MakeLoadOperations(ResourceRepositoryPlan plan) = 0;
    };

    /// Loads resources
    class ResourceRepository : public SomeResourceRepository {
    protected:
        void Run(LoadResourcesOperationList const& operations);

    public:
        using LoadResourcesOperationList = VectorList<SP<SomeLoadResourcesOperation>>;

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

        LoadResourcesOperationList MakeLoadOperations(ResourceInfo info) override;
        LoadResourcesOperationList MakeLoadOperations(ResourceRepositoryPlan plan) override;
    };
} // namespace PJ
