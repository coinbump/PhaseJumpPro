#pragma once

#include "SDKIncludes.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

using namespace PJ;

static void SDLCALL MyNewAudioCallback(
    void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount
) {
    /* Calculate a little more audio here, maybe using `userdata`, write it to `stream`
     *
     * If you want to use the original callback, you could do something like this:
     */
    if (additional_amount > 0) {
        Uint8* data = SDL_stack_alloc(Uint8, additional_amount);
        if (data) {
            // MyAudioCallback(userdata, data, additional_amount);
            SDL_PutAudioStreamData(stream, data, additional_amount);
            SDL_stack_free(data);
        }
    }
}

class TestAudioPad : public WorldComponent {
public:
    using This = TestAudioPad;

    AudioStreamPlayer& audioStreamPlayer;

    TestAudioPad(AudioStreamPlayer& _audioStreamPlayer) :
        audioStreamPlayer(_audioStreamPlayer) {
        AddSignalHandler({ .id = SignalId::PointerDown,
                           .func = [this](auto& component, auto& signal) {
                               audioStreamPlayer.Play();
                           } });
    }

    // MARK: SomeWorldComponent

    String TypeName() const override {
        return "TestAudioPad";
    }
};

class TestAudioSystem : public WorldSystem {
public:
    using Base = WorldSystem;

    TestAudioSystem(String name = "Test Audio") :
        Base(name) {}

    void ProcessUIEvents(UIEventList const& uiEvents) override {
        Base::ProcessUIEvents(uiEvents);

        // double check for memory leaks
        for (auto& event : uiEvents) {
            auto dropFileEvent = DCAST<DropFilesUIEvent>(event);
            if (dropFileEvent) {
                for (auto& filePath : dropFileEvent->filePaths) {
                    // https://wiki.libsdl.org/SDL3/Tutorials/AudioStream
                    // FUTURE: const SDL_AudioSpec outputAudioSpec = { SDL_AUDIO_S16, 2, 48000
                    // };

                    StandardResourceRepositoryModel repoModel;
                    SDLLoadAudioStreamOperation operation(
                        ResourceInfo{ .filePath = filePath, .type = "audio" }, repoModel
                    );
                    operation.Run();

                    auto resourceCatalog = operation.Resources();
                    if (!IsEmpty(resourceCatalog)) {
                        auto stream = SCAST<SDLAudioStream>(resourceCatalog[0].resource);
                        auto playerNode = MAKE<WorldNode>();
                        auto& player = playerNode->With<AudioStreamPlayer>();
                        world->Add(playerNode);

                        auto texture = world->resources.FindTexture("audio-pad");
                        GUARD(texture)
                        auto spriteRenderer = MAKE<SpriteRenderer>(texture);

                        playerNode->Add(spriteRenderer);

                        // do this by default
                        auto thumbMaterial = spriteRenderer->model.material;
                        thumbMaterial->EnableFeature(RenderFeature::Blend, true);

                        QuickBuilder(*playerNode).Drag();

                        auto audioPad = MAKE<TestAudioPad>(player);
                        playerNode->Add(audioPad);

                        player.AddSignalHandler({ .id = "pause",
                                                  .func = [&player](auto& component, auto& signal) {
                                                      player.Pause();
                                                  } });
                        player.AddSignalHandler({ .id = "play",
                                                  .func = [&player](auto& component, auto& signal) {
                                                      player.Play();
                                                  } });

                        player.audioStream = stream;
                        player.Play();
                    }
                }
                continue;
            }
        }
    }
};

/// Test scene for sliced Slice9TextureRenderer
class TestAudioScene : public Scene {
public:
    TestAudioScene() {
        PlanUIFunc planUIFunc = [this](auto args) {
            auto& planner = args.planner;

            planner.Button({ .label = "Pause", .func = [this]() {
                                for (auto& child : owner->Parent()->Children()) {
                                    Event<> event;
                                    child->Signal("pause", event);
                                }
                            } });
            planner.Button({ .label = "Play", .func = [this]() {
                                for (auto& child : owner->Parent()->Children()) {
                                    Event<> event;
                                    child->Signal("play", event);
                                }
                            } });
        };
        Override(planUIFuncs[UIContextId::Editor], planUIFunc);
    }

    void LoadInto(WorldNode& root) override {
        QB(root).Named("TestAudioScene").OrthoStandard();

        World& world = *root.World();

        world.Add(MAKE<TestAudioSystem>());
    }
};
