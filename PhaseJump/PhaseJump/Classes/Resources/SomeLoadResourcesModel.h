#ifndef PJSOMELOADRESOURCESMODEL_H
#define PJSOMELOADRESOURCESMODEL_H

#include "_Map.h"
#include "Factory.h"
#include "SomeLoadResourcesOperation.h"
#include "FactoryRegistry.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    class SomeLoadResourcesOperation;

    class SomeLoadResourcesModel {
    public:
        /// Maps file extension to resource type
        Map<String, String> fileExtensionMap;

        /// Maps resource type to operation
        FactoryRegistry<SomeLoadResourcesOperation> operationRegistry;
    };
}

#endif
