#ifndef PJLOADRESOURCESPLAN_H
#define PJLOADRESOURCESPLAN_H

#include "FilePath.h"
#include "OrderedMap.h"
#include "ResourceInfo.h"
#include "StringUtils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    using ResourceType = String;
    using ResourceId = String;

    // TODO: should the second key be the file path? might have multiple resources with the same id
    // in different folders?
    using ResourceInfoMap = OrderedMap<ResourceType, OrderedMap<ResourceId, ResourceInfo>>;

    /// Plan to load a collection of resources
    struct LoadResourcesPlan {
        ResourceInfoMap infoMap;

        VectorList<ResourceInfo> AllInfos() const {
            VectorList<ResourceInfo> result;

            for (auto& i : infoMap) {
                auto& secondMap = i.second;
                for (auto& j : secondMap) {
                    auto info = j.second;
                    Add(result, info);
                }
            }

            return result;
        }
    };
} // namespace PJ

#endif
