#pragma once

#include "Base.h"
#include "FilePath.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// Stores metadata for resource that was loaded
    struct LoadedResource {
        /// Path of the resource
        FilePath filePath;

        /// Each supported file extension is mapped to a type. The type determines how to load
        /// the resource Allows for multiple file types to use the same load operation ("png",
        /// "jpg", "jpeg" load a texture)
        String type;

        /// Unique identifier for this resource (default: uses file name without extension)
        String id;

        /// The resource that was loaded
        SP<Base> resource;

        LoadedResource() {}

        LoadedResource(FilePath filePath, String type, String id, SP<Base> resource) :
            filePath(filePath),
            type(type),
            id(id),
            resource(resource) {}
    };

    /// First key: resource type, second key: id
    using LoadedResourceMap = UnorderedMap<String, UnorderedMap<String, LoadedResource>>;

    struct LoadedResources {
        LoadedResourceMap map;

        void Remove(String type, String id) {
            auto i = map.find(type);
            GUARD(i != map.end())

            auto& secondMap = i->second;
            auto j = secondMap.find(id);
            GUARD(j != secondMap.end())

            secondMap.erase(j);
        }
    };
} // namespace PJ
