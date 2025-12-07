#pragma once

#include "SDKIncludes.h"

using namespace PJ;

namespace Kaiju {
    /// Accepts files via drag and drop and runs file processor logic
    class FilesProcessorScene : public Scene {
    public:
        FilesProcessorScene() {}

        void LoadInto(WorldNode& root) override;
    };
} // namespace Kaiju
