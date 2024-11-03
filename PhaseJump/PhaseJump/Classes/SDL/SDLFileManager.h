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
    /// Provides file manager functionality via SDL
    class SDLFileManager : public FileManager {
    public:
        // MARK: SomeFileManager

        FilePath PersistentDataPath(String companyName, String applicationName) override {
            return SDL_GetPrefPath(companyName.c_str(), applicationName.c_str());
        }
    };
} // namespace PJ
