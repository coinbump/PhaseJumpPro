#pragma once

#include "Factory.h"
#include "FactoryRegistry.h"
#include "LoadResourcesOperation.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    class LoadResourcesOperation;
    class ResourceRepositoryModel;

    /**
     A model for loading resources from a resource repository

     Each file extension is mapped to a resource type

     Each resource type is mapped to a factory that produces a load resources operation

     Each load resources operation can load resources
     */
    class ResourceRepositoryModel {
    protected:
        /// Maps file extension to resource type
        UnorderedMap<String, String> fileExtensionMap;

        /// Maps resource type to operation
        FactoryRegistry<LoadResourcesOperation, ResourceInfo, ResourceRepositoryModel&>
            operationRegistry;

    public:
        using LoadResourcesOperationList = VectorList<SP<LoadResourcesOperation>>;

        virtual ~ResourceRepositoryModel() {}

        virtual std::optional<String> TypeForFileExtension(String fileExtension);

        virtual LoadResourcesOperationList MakeLoadOperations(ResourceInfo info);
    };
} // namespace PJ
