#include "MediaTracksAppScene.h"

using namespace std;
using namespace PJ;

namespace {
    constexpr float timelineWidth = 800.0f;
    constexpr float timelineHeight = 800.0f;
    constexpr int trackCount = 5;
    constexpr float timelineDuration = 180.0f; // 3 minutes

    Color TrackBackgroundColor(int trackIndex) {
        return (trackIndex % 2 == 0) ? Color(0.85f, 0.85f, 0.85f, 1.0f)
                                     : Color(0.65f, 0.65f, 0.65f, 1.0f);
    }

    Color ClipColorForTrack(int trackIndex) {
        switch (trackIndex) {
        case 0:
            return Color::red;
        case 1:
            return Color::green;
        case 2:
            return Color::blue;
        case 3:
            return Color::yellow;
        default:
            return Color(0.8f, 0.3f, 0.8f, 1.0f); // purple
        }
    }
} // namespace

MediaTracksAppScene::MediaTracksAppScene() {
    fileManager = NewMacFileManager();
}

void MediaTracksAppScene::LoadInto(WorldNode& root) {
    QB(root).Named("Media Tracks App").OrthoStandard().SizeWithWindow();

    // Root view container. View2D::ApplyLayout only runs a root view's own layout on child[0]
    // (others are treated as top-level overlays), so we put the timeline inside a dedicated
    // root and let layoutRecurse apply the timeline's VStack to all tracks.
    auto& rootViewNode = root.And(WorldNode::Config{ .name = "Media Tracks Root" });
    auto& rootView = rootViewNode.AddComponent<View2D>();
    rootView.SetFrameSize({ timelineWidth, timelineHeight });

    auto& timelineNode = rootViewNode.And(WorldNode::Config{ .name = "Timeline" });
    auto& timeline = timelineNode.AddComponent<MediaTimelineView>();
    timeline.TimelineModel().duration = timelineDuration;
    timelineView = SCAST<MediaTimelineView>(timeline.shared_from_this());

    timeline.buildClipViewFunc = [this](MediaTimelineView::BuildClipViewArgs const& args) {
        auto color = ClipColorForTrack(args.trackIndex);

        // Clip body + waveform renderer are siblings on the clip node so they get resized
        // together by View2D::UpdateFrameComponents whenever the clip's frame changes.
        args.clipNode.AddComponent<ColorRenderer>(ColorRenderer::Config{ .color = color });

        auto& renderer =
            args.clipNode.AddComponent<SDLAudioStreamRenderer>(SDLAudioStreamRenderer::Config{
                .worldSize = { 0, 0 },
                .maxSegmentsCount = 400,
                .color = Color::black,
                .waveformType = WaveformType::MinMax,
                .streamWindow = StreamWindow(args.clip->sourceStart, args.clip->duration) });

        GUARD(!args.clip->id.empty())
        auto audioStream = resources.Find<SDLAudioStream>(ResourceType::AudioStream, args.clip->id);
        if (!audioStream) {
            FilePath filePath(args.clip->id);
            ResourceRepository repo(repoModel, resources, *fileManager);
            ResourceInfo info{ .id = args.clip->id,
                               .filePath = filePath,
                               .type = ResourceType::AudioStream };
            repo.Load(info);
            audioStream = resources.Find<SDLAudioStream>(ResourceType::AudioStream, args.clip->id);
        }
        if (audioStream) {
            renderer.SetAudioStream(audioStream);
        }
    };

    for (int i = 0; i < trackCount; ++i) {
        auto& track = timeline.AddTrack();
        auto& trackNode = *track.owner;
        trackNode.AddComponent<ColorRenderer>(ColorRenderer::Config{ .color =
                                                                         TrackBackgroundColor(i) });
    }

    auto dropFilesHandler = [this](SomeWorldComponent& owner, DropFilesUIEvent const& event) {
        auto timeline = LOCK(timelineView);
        GUARD(timeline)

        auto world = owner.Node() ? owner.Node()->World() : nullptr;
        GUARD(world)

        Vector2 worldPosition{};
        auto worldPos3 = ScreenToWorld(owner, event.position);
        worldPosition = { worldPos3.x, worldPos3.y };

        for (auto& filePath : event.filePaths) {
            auto extension =
                fileManager->FileExtension(filePath, FileManager::FileExtensionFormat::NoDot);
            GUARD_CONTINUE(extension == "wav" || extension == "WAV")

            auto clip = MAKE<MediaClipModel>();
            clip->id = filePath;
            clip->start = 0;
            clip->sourceStart = 0;

            // Try to determine the clip duration from the audio file. If we can't, use a
            // 10-second placeholder so it's visible on the track.
            ResourceRepository repo(repoModel, resources, *fileManager);
            ResourceInfo info{ .id = filePath,
                               .filePath = filePath,
                               .type = ResourceType::AudioStream };
            repo.Load(info);
            auto audioStream = resources.Find<SDLAudioStream>(ResourceType::AudioStream, filePath);
            if (audioStream) {
                auto spec = audioStream->AudioSpec();
                auto bytesPerSample = SDL_AUDIO_BYTESIZE(spec.format) * spec.channels;
                if (spec.freq > 0 && bytesPerSample > 0) {
                    clip->duration = static_cast<float>(audioStream->BufferLength()) /
                                     static_cast<float>(spec.freq * bytesPerSample);
                }
            }
            if (clip->duration <= 0) {
                clip->duration = 10.0f;
            }

            timeline->AddClip(worldPosition, clip);
        }
    };

    AddSignalHandler<DropFilesUIEvent>({ .id = SignalId::FilesDrop, .func = dropFilesHandler });
}
