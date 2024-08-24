#pragma once

#include <PhaseJump/PhaseJump.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

using namespace PJ;

static void SDLCALL MyNewAudioCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount)
{
    /* Calculate a little more audio here, maybe using `userdata`, write it to `stream`
     *
     * If you want to use the original callback, you could do something like this:
     */
    if (additional_amount > 0) {
        Uint8 *data = SDL_stack_alloc(Uint8, additional_amount);
        if (data) {
            //MyAudioCallback(userdata, data, additional_amount);
            SDL_PutAudioStreamData(stream, data, additional_amount);
            SDL_stack_free(data);
        }
    }
}

class TestAudioPad : public WorldComponent<>, public SomePointerUIEventsResponder {
public:
    SP<AudioStreamPlayer> audioStreamPlayer;

    TestAudioPad(SP<AudioStreamPlayer> audioStreamPlayer) : audioStreamPlayer(audioStreamPlayer) {
    }

    void OnPointerDown(PointerDownUIEvent event) {
        GUARD(audioStreamPlayer);
        audioStreamPlayer->Play();
    }
};


class TestAudioSystem : public SomeWorldSystem {
public:
    using Base = SomeWorldSystem;

    void ProcessUIEvents(List<SP<SomeUIEvent>> const& uiEvents) override {
        Base::ProcessUIEvents(uiEvents);

        // TODO: double check for memory leaks
        for (auto& event : uiEvents) {
            auto dropFileEvent = DCAST<DropFilesUIEvent>(event);
            if (dropFileEvent) {
                for (auto& filePath : dropFileEvent->filePaths) {
                    SDL_AudioSpec inputAudioSpec;
                    Uint32 wav_length;
                    Uint8 *wav_buffer;

                    // TODO: SDL_ClearAudioStream, DestroyAudioStream

                    // https://wiki.libsdl.org/SDL3/Tutorials/AudioStream
                    if (SDL_LoadWAV(filePath.c_str(), &inputAudioSpec, &wav_buffer, &wav_length) == 0) {
                        // FUTURE: const SDL_AudioSpec outputAudioSpec = { SDL_AUDIO_S16, 2, 48000 };

                        StandardLoadResourcesModel loadResourcesModel(StandardLoadResourcesModel::LoadType::Rez);
                        SDLLoadAudioStreamOperation operation(LoadResourceInfo(filePath, "audio", ""), loadResourcesModel);
                        operation.Run();

                        auto loadedResources = operation.LoadedResources();
                        if (!IsEmpty(loadedResources)) {
                            auto stream = SCAST<SDLAudioStream>(loadedResources[0].resource);
                            auto player = MAKE<AudioStreamPlayer>();
                            auto playerNode = MAKE<WorldNode>();
                            playerNode->Add(player);
                            world->Add(playerNode);

                            auto texture = DCAST<GLTexture>(world->loadedResources->map["texture"]["audio-pad"].resource);
                            GUARD(texture)
                            auto spriteRenderer = MAKE<SpriteRenderer>(texture);

                            playerNode->Add(spriteRenderer);

                            // TODO: do this by default
                            auto thumbMaterial = spriteRenderer->material;
                            thumbMaterial->SetShaderProgram(SomeShaderProgram::registry["texture.uniform"]);
                            thumbMaterial->EnableFeature(RenderFeature::Blend, true);
                            
                            ComponentTool ct;
                            auto collider = MAKE<PolygonCollider2D>();
                            ct.AddComponent(*playerNode, collider);

                            auto dragHandler = MAKE<DragHandler2D>();
                            playerNode->Add(dragHandler);

                            auto audioPad = MAKE<TestAudioPad>(player);
                            playerNode->Add(audioPad);

                            // TODO: playerNode->SetWorldPosition(

                            player->audioStream = stream;
                            player->Play();
                        }
                    }
                }
                continue;
            }
        }
    }
};

/// Test scene for sliced SlicedTextureRenderer
class TestAudioScene : public Scene {
public:
    TestAudioScene() {
    }

    void LoadInto(WorldNode& root) {
        root.name = "TestAudioScene";

        auto camera = SCAST<SomeCamera>(MAKE<OrthoCamera>());
        auto cameraNode = MAKE<WorldNode>();
        cameraNode->Add(camera);
        root.Add(cameraNode);

        auto raycaster = MAKE<SimpleRaycaster2D>();
        cameraNode->Add(raycaster);
        root.Add(cameraNode);

        World& world = *root.World();

        auto uiSystem = MAKE<UISystem>();
        world.Add(uiSystem);

        // TODO: clean this up somewhere
        world.Add(MAKE<TestAudioSystem>());
    }
};
