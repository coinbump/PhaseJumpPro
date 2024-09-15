#pragma once

#include "Factory.h"
#include "FactoryRegistry.h"
#include "SomeLoadResourcesOperation.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    class SomeLoadResourcesOperation;
    class LoadResourcesModel;

    /**
     A model for loading resources from a resource repository

     Each file extension is mapped to a resource type

     Each resource type is mapped to a factory that produces a load resources operation

     Each load resources operation can load resources
     */
    class LoadResourcesModel {
    protected:
        /// Maps file extension to resource type
        UnorderedMap<String, String> fileExtensionMap;

        /// Maps resource type to operation
        FactoryRegistry<SomeLoadResourcesOperation, ResourceInfo, LoadResourcesModel&>
            operationRegistry;

    public:
        virtual ~LoadResourcesModel() {}

        virtual std::optional<String> TypeIdForFileExtension(String fileExtension) {
            auto iterator = fileExtensionMap.find(fileExtension);
            GUARDR(iterator != fileExtensionMap.end(), std::nullopt)

            return iterator->second;
        }

        // TODO: should be UP
        virtual List<SP<SomeLoadResourcesOperation>> MakeLoadOperations(ResourceInfo info) {
            List<SP<SomeLoadResourcesOperation>> result;

            // FUTURE: support multiple load operations if needed
            auto operation = operationRegistry.New(info.type, info, *this);
            GUARDR(operation, result)

            Add(result, operation);
            return result;
        }
    };
} // namespace PJ
