#pragma once

#include "FilePath.h"
#include "OrderedMap.h"
#include "PropertyIdBuilder.h"
#include "ResourceInfo.h"
#include "StringUtils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    using ResourceId = String;

    // Maps file paths to info for that resource
    using ResourceInfoMap = OrderedMap<String, ResourceInfo>;

    /// Plan to load a collection of resources
    struct ResourceRepositoryPlan {
        VectorList<ResourceInfo> infos;
    };
} // namespace PJ
