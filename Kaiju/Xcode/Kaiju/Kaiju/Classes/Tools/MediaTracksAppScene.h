#pragma once

#include "SDKIncludes.h"

namespace PJ {
    /// DAW-style multitrack timeline tool. Drop .wav files onto tracks to insert audio clips
    /// with waveform rendering. New clips split or overwrite overlapping existing clips.
    class MediaTracksAppScene : public Scene {
    public:
        MediaTracksAppScene();

        // MARK: Scene

        void LoadInto(WorldNode& root) override;

    protected:
        UP<FileManager> fileManager;
        WP<MediaTimelineView> timelineView;

        StandardResourceRepositoryModel repoModel;
        ResourceCatalog resources;
    };
} // namespace PJ
