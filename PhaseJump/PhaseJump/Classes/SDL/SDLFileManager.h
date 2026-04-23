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
        result->persistentDataPathFunc = [](String companyName,
                                            String applicationName) -> FilePath {
            // SDL_GetPrefPath must be released with SDL_free
            char* raw = SDL_GetPrefPath(companyName.c_str(), applicationName.c_str());
            GUARDR(raw, FilePath{})

            FilePath pathResult{ raw };
            SDL_free(raw);
            return pathResult;
        };

        return result;
    }
} // namespace PJ
