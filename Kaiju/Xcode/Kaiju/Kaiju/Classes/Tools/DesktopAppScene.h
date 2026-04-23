#pragma once

#include "SDKIncludes.h"

namespace PJ {
    class Desktop;

    /// Demonstrates Window renders on a Desktop
    class DesktopAppScene : public Scene {
    public:
        DesktopAppScene();

        // MARK: Scene

        void LoadInto(WorldNode& root) override;

    protected:
        WP<Desktop> desktop;
        UP<FileManager> fileManager;

        void NewWindow();
        void NewCheckersWindow();
        void NewSceneWindow(SceneClass* sceneClass);
        void NewAudioWindow(FilePath filePath, Vector2 origin);
        void NewImageWindow(FilePath filePath, Vector2 origin);
        void NewMovieWindow(FilePath filePath, Vector2 origin);

        /// Converts a screen position (logical points, top-left origin, +y down) into a
        /// desktop reading-coordinate origin (top-left of desktop = {0,0}, +y down, in
        /// world units), going through the main camera so DPI scaling is handled.
        Vector2 ScreenPositionToReadingOrigin(Vector2 screenPosition) const;
    };
} // namespace PJ
