#pragma once

#include "FileManager.h"
#include "StringUtils.h"
#include <SDL3/SDL_filesystem.h>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 10/6/24
 */
namespace PJ {
    static UP<FileManager> NewSDLFileManager() {
        UP<FileManager> result = NEW<FileManager>();
        result->persistentDataPathFunc = [](String companyName, String applicationName) {
            return SDL_GetPrefPath(companyName.c_str(), applicationName.c_str());
        };

        return result;
    }
} // namespace PJ
