#include "AudioAppScene.h"

using namespace std;
using namespace PJ;

namespace {
    auto constexpr textNodeId = "text";
    auto constexpr waveformNodeId = "waveform";
} // namespace

void AudioAppScene::UpdateTextNode() {
    auto node = Node();
    GUARD(node)

    auto world = node->World();
    GUARD(world)

    auto root = this->Node();
    GUARD(root)

    SP<AudioAppScene::Document> activeDocument =
        DCAST<AudioAppScene::Document>(documents.ActiveDocument());

    auto textNode = (*root)[textNodeId];
    if (textNode) {
        auto textRenderer = textNode->TypeComponent<TextRenderer>();
        if (textRenderer) {
            String text = activeDocument ? activeDocument->name : "";
            textRenderer->SetPlainText(text);
        }
    }

    auto waveformNode = (*root)[waveformNodeId];
    if (waveformNode) {
        auto waveformRenderer = waveformNode->TypeComponent<SDLAudioStreamRenderer>();
        if (waveformRenderer) {
            SP<SDLAudioStream> audioStream;
            if (activeDocument) {
                audioStream = DCAST<SDLAudioStream>(activeDocument->core.audioStream);
            }
            waveformRenderer->SetAudioStream(audioStream);
        }
    }
}

AudioAppScene::AudioAppScene() {
    // TODO: make this platform agnostic
    fileManager = NewMacFileManager();

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        planner.ListSelect([this]() {
            VectorList<String> options =
                Map<String>(documents.Documents(), [](auto& document) { return document->name; });

            Binding<int> binding(
                { .getFunc =
                      [this]() {
                          auto result = IndexOfIf(documents.Documents(), [this](auto& document) {
                              auto activeDocument = documents.ActiveDocument();
                              return document.get() == activeDocument.get();
                          });
                          return result ? *result : -1;
                      },
                  .setFunc =
                      [=, this](auto& value) {
                          GUARD(value >= 0 && value < documents.Count())
                          auto document = documents.Documents()[value];
                          documents.SetActiveDocument(document);
                          UpdateTextNode();
                      } }
            );

            return UIPlanner::ListSelectConfig{ .label = "Documents",
                                                .options = options,
                                                .binding = binding };
        });

        planner.Button({ .label = "Play", .func = [this]() {
                            SP<AudioAppScene::Document> activeDocument =
                                DCAST<AudioAppScene::Document>(documents.ActiveDocument());
                            GUARD(activeDocument && activeDocument->core.audioStream)
                            activeDocument->core.audioStream->Play();
                        } });
        planner.Button({ .label = "Pause", .func = [this]() {
                            SP<AudioAppScene::Document> activeDocument =
                                DCAST<AudioAppScene::Document>(documents.ActiveDocument());
                            GUARD(activeDocument && activeDocument->core.audioStream)
                            activeDocument->core.audioStream->Pause();
                        } });

        if (fileManager->showFileFunc) {
            planner.Button({ .label = "Show file", .func = [this]() {
                                SP<AudioAppScene::Document> activeDocument =
                                    DCAST<AudioAppScene::Document>(documents.ActiveDocument());
                                GUARD(activeDocument)

                                fileManager->ShowFile(activeDocument->GetFilePath());
                            } });
        }

        planner.Button({ .label = "Close", .func = [this]() {
                            documents.CloseActiveDocument();
                            UpdateTextNode();
                        } });
    };

    Override(planUIFuncs[UIContextId::Editor], planUIFunc);

    auto dropFilesHandler = [this](SomeWorldComponent& owner, DropFilesUIEvent const& event) {
        for (auto& filePath : event.filePaths) {
            Document::Config documentConfig{
                .name = fileManager->FileName(filePath, FileManager::FileNameFormat::NoExtension),
                .filePath = filePath,
            };
            SP<Document> document = MAKE<Document>(documentConfig);

            StandardResourceRepositoryModel repoModel;

            ResourceRepository repo(repoModel, document->resources, *fileManager);
            ResourceInfo info{ .id = filePath,
                               .filePath = filePath,
                               .type = ResourceType::AudioStream };
            repo.Load(info);

            auto audioStream =
                document->resources.TypeFirst<SDLAudioStream>(ResourceType::AudioStream);
            if (audioStream) {
                document->core.audioStream = audioStream;
                documents.Add(document);
                documents.SetActiveDocument(document);
                UpdateTextNode();
            }
        }
    };

    AddSignalHandler<DropFilesUIEvent>({ .id = SignalId::FilesDrop, .func = dropFilesHandler });

    AddSignalHandler<KeyDownUIEvent>({ .id = SignalId::KeyDown,
                                       .func = [this](auto& component, auto& keyDownEvent) {
                                           switch (keyDownEvent.KeyCodeValue()) {
                                           case SDLK_UP:
                                               documents.ActivatePreviousDocument(WrapType::Wrap);
                                               UpdateTextNode();
                                               break;
                                           case SDLK_DOWN:
                                               documents.ActivateNextDocument(WrapType::Wrap);
                                               UpdateTextNode();
                                               break;
                                           }
                                       } });
}

void AudioAppScene::LoadInto(WorldNode& root) {
    QB(root).Named("Audio App").OrthoStandard().SizeWithWindow();

    QB(root)
        .And(WorldNode::Config{ .id = textNodeId, .name = "Text" })
        .With<TextRenderer>(TextRenderer::Config{})
        .With<AudioStreamPlayer>();
    QB(root)
        .And(WorldNode::Config{ .id = waveformNodeId, .name = "Waveform" })
        .With<SDLAudioStreamRenderer>(SDLAudioStreamRenderer::Config{
            .worldSize = { 900, 300 },
            .maxSegmentsCount = 2000,
            .color = Color::red,
        });
}
