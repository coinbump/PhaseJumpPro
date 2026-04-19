#pragma once

#include "SDKIncludes.h"

namespace PJ {
    /// Audio player tool app
    class AudioAppScene : public Scene {
    public:
        class DocumentCore {
        public:
            SP<SomeAudioStream> audioStream;
        };

        using Document = PJ::CoreDocument<DocumentCore>;

        DocumentBundle documents;
        UP<FileManager> fileManager;

        AudioAppScene();

        // MARK: Scene

        void LoadInto(WorldNode& root) override;

    protected:
        void UpdateTextNode();
    };
} // namespace PJ
