#pragma once

#include "Base.h"
#include "FilePath.h"
#include "ResourceInfo.h"
#include "UnorderedMap.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/10/24
 */
namespace PJ {
    /// Bundles resource and resource info
    struct ResourceModel {
        /// The resource
        SP<Base> resource;

        ResourceInfo info;

        ResourceModel() {}

        ResourceModel(SP<Base> resource, String id, FilePath filePath, String type) :
            info(id, filePath, type),
            resource(resource) {}

        ResourceModel(SP<Base> resource, ResourceInfo info) :
            info(info),
            resource(resource) {}
    };

    /// First key: resource type, second key: id
    using ResourceModelMap = UnorderedMap<String, UnorderedMap<String, ResourceModel>>;

    struct ResourceModels {
        ResourceModelMap map;

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
