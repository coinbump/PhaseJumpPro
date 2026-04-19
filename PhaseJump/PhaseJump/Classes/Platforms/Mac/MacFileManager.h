#pragma once

#include "SDLFileManager.h"

namespace PJ {
    /// Creates a file manager with Mac-specific functionality
    UP<FileManager> NewMacFileManager();
} // namespace PJ
