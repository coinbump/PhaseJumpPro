#pragma once

#include "SDKIncludes.h"

namespace PJ {
    /// Edit image tool app
    class ImageAppScene : public Scene {
    public:
        class DocumentCore {
        public:
            SP<RGBABitmap> bitmap;
            SP<Texture> texture;
        };

        using Document = PJ::CoreDocument<DocumentCore>;

        class CommandCore {
        public:
            SP<Document> document;
        };

        using Command = PJ::Command<CommandCore>;
        using CommandClass = PJ::CommandClass<CommandCore>;

        DocumentBundle documents;
        UP<FileManager> fileManager;

        ClassRegistry<CommandClass> commandClasses;

        ImageAppScene();

        // MARK: Scene

        void LoadInto(WorldNode& root) override;

    protected:
        void RegisterClasses();
        void Run(SP<Document> document, BitmapOperation& operation);
    };
} // namespace PJ
