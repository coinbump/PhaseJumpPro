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

        DocumentBundle documents;
        SP<Document> activeDocument;
        SP<Texture> activeTexture;

        ClassRegistry<BitmapOperationClass> operationClasses;

        ImageAppScene();

        void SetActiveDocument(SP<Document> document) {
            GUARD(activeDocument != document)
            activeDocument = document;
        }

        void NavigateToPreviousDocument();
        void NavigateToNextDocument();

        // MARK: Scene

        void LoadInto(WorldNode& root) override;

    protected:
        void RegisterOperationClasses();

        /*
         on select bitmap operation class -> create bitmap operation -> create command -> add to
         commands for undo/redo

         ***??? when bitmap changes, update texture with new pixels + bump modified count <- or just
         create a new one? What if we cropped the bitmap?

         ImRenderer draws frame + dropShadow + image

         Tests: Document, DocumentBundle, file
         */

        // DocumentBundle documentBundle;
    };
} // namespace PJ
