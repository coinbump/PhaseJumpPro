#ifndef PJLOADEDRESOURCES_H
#define PJLOADEDRESOURCES_H

#include "Base.h"
#include "FilePath.h"
#include "_Map.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    /// Stores metadata for resource that was loaded
    struct LoadedResource {
        FilePath filePath;
        String typeId;
        String id;
        SP<Base> resource;

        LoadedResource() {
        }
        
        LoadedResource(FilePath filePath, String typeId, String id, SP<Base> resource) :
        filePath(filePath),
        typeId(typeId),
        id(id),
        resource(resource) {
        }
    };

    /// First key: resource type, second key: id
    using LoadedResourceMap = Map<String, Map<String, LoadedResource>>;

    struct LoadedResources {
        LoadedResourceMap map;

        void Remove(String typeId, String id) {
            auto i = map.find(typeId);
            if (i != map.end()) {
                auto & secondMap = i->second;
                auto j = secondMap.find(id);
                if (j != secondMap.end()) {
                    secondMap.erase(j);
                }
            }
        }
    };
}

#endif
