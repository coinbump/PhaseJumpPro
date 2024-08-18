#pragma once

#include "_String.h"
#include "FilePath.h"
#include "Tags.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    class LoadResourcesModel;

    /// Specifies information for loading an individual resource
    struct LoadResourceInfo {
        /// Path of the resource
        FilePath filePath;

        /**
         Each supported file extension is mapped to a resource type. The type determines how to
         load the resource

         Allows for multiple file types to use the same load operation (Example: "png", "jpg",
         "jpeg" load a "texture" resource type)
         */
        String type;

        /// Unique identifier for this resource (default: uses file name without extension)
        String id;

        /// Custom arguments
        Tags args;

        LoadResourceInfo() {}

        LoadResourceInfo(FilePath filePath, StringC type, StringC id) :
            filePath(filePath),
            type(type),
            id(id) {}
    };
} // namespace PJ
