#ifndef SDLFILEMANAGER_H
#define SDLFILEMANAGER_H

#include <SDL2/SDL_filesystem.h>
#include "_String.h"

namespace PJ {
    class SDLFileManager
    {
        String PersistentDataPath(String companyName, String applicationName) const
        {
            return String(SDL_GetPrefPath(companyName.c_str(), applicationName.c_str()));
        }
    };
}

#endif
