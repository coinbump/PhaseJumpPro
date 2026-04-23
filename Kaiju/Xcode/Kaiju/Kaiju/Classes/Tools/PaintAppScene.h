#pragma once

#include "SDKIncludes.h"

namespace PJ {
    /// Paint tool app: drag-to-paint with optional image load/save. The active document in
    /// `documents` carries the on-disk file path, so subsequent Saves reuse it.
    class PaintAppScene : public Scene {
    public:
        class DocumentCore {
        public:
            SP<RGBABitmap> bitmap;
        };

        using Document = PJ::CoreDocument<DocumentCore>;

        DocumentBundle documents;
        UP<FileManager> fileManager;

        PaintAppScene();

        // MARK: Scene

        void LoadInto(WorldNode& root) override;

        /// Loads the PNG at `filePath` into the canvas, replacing its contents.
        /// Exposed so the async SDL open-file callback can call back in.
        void LoadImageFromPath(FilePath filePath);

        /// Writes the current canvas buffer to `filePath` as a PNG and updates/creates the
        /// active Document in the bundle. Exposed so the async save-file callback can call it.
        void WriteCanvasToPath(FilePath filePath);

    protected:
        WP<PaintCanvas> paintCanvas;

        /// Writes to the active document's file path if present; otherwise shows the save dialog.
        void SaveCanvas();

        /// Always shows the save dialog.
        void SaveCanvasAs();

        /// Opens the file dialog to load a PNG.
        void LoadImageDialog();
    };
} // namespace PJ
