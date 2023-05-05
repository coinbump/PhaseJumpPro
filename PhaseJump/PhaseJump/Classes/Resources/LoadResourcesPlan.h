#ifndef PJLOADRESOURCESPLAN_H
#define PJLOADRESOURCESPLAN_H

#include "LoadResourceInfo.h"
#include "FilePath.h"
#include "_String.h"
#include "_Map.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    using ResourceInfoMap = Map<String, Map<String, LoadResourceInfo>>;

    /// Plan to load a collection of resources
    struct LoadResourcesPlan {
        // First key: resource type. Second key: id
        ResourceInfoMap infoMap;

        VectorList<LoadResourceInfo> AllInfos() const {
            VectorList<LoadResourceInfo> result;

            for (auto i : infoMap) {
                auto & secondMap = i.second;
                for (auto j: secondMap) {
                    auto info = j.second;
                    result.Add(info);
                }
            }

            return result;
        }
    };
}

#endif
