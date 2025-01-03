#pragma once

#include "FilePath.h"
#include "StringUtils.h"
#include "Tags.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// Specifies information for loading an individual resource
    struct ResourceInfo {
        /// Unique identifier for this resource (default: uses file name without extension)
        String id;

        /// Path of the resource
        FilePath filePath;

        /**
         Each supported file extension is mapped to a resource type. The type determines how to
         load the resource

         Allows for multiple file types to use the same load operation (Example: "png", "jpg",
         "jpeg" load a ResourceType::Texture resource type)
         */
        String type;

        /// Custom arguments for loading
        Tags loadArgs;
    };
} // namespace PJ
