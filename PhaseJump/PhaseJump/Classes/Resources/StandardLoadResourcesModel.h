#pragma once

#include "LoadResourcesModel.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    class StandardLoadResourcesModel : public LoadResourcesModel {
    public:
        enum class LoadType {
            /// Load .rez files with metadata that describes each resource
            Rez,

            /// Scan all files and load (won't work with complex resource types)
            Scan
        };

        StandardLoadResourcesModel(LoadType loadType);
    };
} // namespace PJ
