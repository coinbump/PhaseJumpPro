#ifndef PJLOADRESOURCEINFO_H
#define PJLOADRESOURCEINFO_H

#include "FilePath.h"
#include "_String.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    /// Specifies information for loading an individual resource
    struct LoadResourceInfo {
        FilePath filePath;
        String typeId;
        String id;

        LoadResourceInfo() {
        }

        LoadResourceInfo(FilePath filePath, StringC typeId, StringC id)
        : filePath(filePath), typeId(typeId), id(id) {
        }
    };
}

#endif
