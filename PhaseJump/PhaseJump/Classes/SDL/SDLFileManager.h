#pragma once
#include "StringUtils.h"
#include <SDL3/SDL_filesystem.h>

// CODE REVIEW: ?/23
namespace PJ {
    class SDLFileManager {
        String PersistentDataPath(String companyName, String applicationName) const {
            return String(SDL_GetPrefPath(companyName.c_str(), applicationName.c_str()));
        }
    };
} // namespace PJ
