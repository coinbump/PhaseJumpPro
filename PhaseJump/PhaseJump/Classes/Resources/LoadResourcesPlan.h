#ifndef PJLOADRESOURCESPLAN_H
#define PJLOADRESOURCESPLAN_H

#include "_String.h"
#include "FilePath.h"
#include "LoadResourceInfo.h"
#include "OrderedMap.h"
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
    using ResourceInfoMap = OrderedMap<ResourceType, OrderedMap<ResourceId, LoadResourceInfo>>;

    /// Plan to load a collection of resources
    struct LoadResourcesPlan {
        ResourceInfoMap infoMap;

        VectorList<LoadResourceInfo> AllInfos() const {
            VectorList<LoadResourceInfo> result;

            for (auto& i : infoMap) {
                auto& secondMap = i.second;
                for (auto& j : secondMap) {
                    auto info = j.second;
                    result.Add(info);
                }
            }

            return result;
        }
    };
} // namespace PJ

#endif
