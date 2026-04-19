#include "ImageAppScene.h"

using namespace std;
using namespace PJ;

ImageAppScene::ImageAppScene() {
    fileManager = NewMacFileManager();

    RegisterClasses();

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        {
            planner.ListSelect([this]() {
                VectorList<String> options = Map<String>(documents.Documents(), [](auto& document) {
                    return document->name;
                });

                Binding<int> binding(
                    { .getFunc =
                          [this]() {
                              auto result =
                                  IndexOfIf(documents.Documents(), [this](auto& document) {
                                      SP<ImageAppScene::Document> activeDocument =
                                          DCAST<ImageAppScene::Document>(documents.ActiveDocument()
                                          );
                                      return document.get() == activeDocument.get();
                                  });
                              return result ? *result : -1;
                          },
                      .setFunc =
                          [=, this](auto& value) {
                              GUARD(value >= 0 && value < documents.Count())
                              auto document = SCAST<Document>(documents.Documents()[value]);
                              documents.SetActiveDocument(document);
                          } }
                );

                return UIPlanner::ListSelectConfig{ .label = "Documents",
                                                    .options = options,
                                                    .binding = binding };
            });
        }

        planner.ListSelect([this]() {
            VectorList<CommandClass*> commandClasses;
            for (auto& commandClass : this->commandClasses.Map()) {
                commandClasses.push_back(commandClass.second.get());
            }

            VectorList<String> options = Map<String>(commandClasses, [](auto& _class) {
                auto command = _class->New();
                return command ? command->Name() : "";
            });

            Binding<int> binding(
                { .getFunc = [this]() { return -1; },
                  .setFunc =
                      [=, this](auto& value) {
                          SP<ImageAppScene::Document> activeDocument =
                              DCAST<ImageAppScene::Document>(documents.ActiveDocument());
                          GUARD(activeDocument && activeDocument->core.bitmap)
                          GUARD(value >= 0 && value < commandClasses.size())

                          auto bitmap = activeDocument->core.bitmap;

                          auto _class = commandClasses[value];
                          auto command = _class->New();
                          GUARD(command)

                          auto editorSystem =
                              this->Node()->World()->TypeSystem<EditorWorldSystem>();
                          GUARD_LOG(editorSystem, "Error: missing editor system for commands")
                          editorSystem->commands.Run(std::move(command));
                      } }
            );

            return UIPlanner::ListSelectConfig{ .label = "Operations",
                                                .options = options,
                                                .binding = binding };
        });

        planner.Button({ .label = "Save", .func = [this]() { documents.SaveActiveDocument(); } });

        if (fileManager->showFileFunc) {
            planner.Button({ .label = "Show file", .func = [this]() {
                                SP<ImageAppScene::Document> activeDocument =
                                    DCAST<ImageAppScene::Document>(documents.ActiveDocument());
                                GUARD(activeDocument)

                                fileManager->ShowFile(activeDocument->GetFilePath());
                            } });
        }

        planner.Button({ .label = "Close", .func = [this]() {
                            auto activeDocument = documents.ActiveDocument();
                            GUARD(activeDocument)

                            // Flush commands for the closed document
                            auto editorSystem =
                                this->Node()->World()->TypeSystem<EditorWorldSystem>();
                            if (editorSystem) {
                                editorSystem->commands.RemoveIf([=](auto& _command) {
                                    auto command = dynamic_cast<Command*>(&_command);
                                    GUARDR(command, false)
                                    return command->core.document == activeDocument;
                                });
                            }

                            documents.CloseActiveDocument();
                        } });
    };

    Override(planUIFuncs[UIContextId::Editor], planUIFunc);

    auto dropFilesHandler = [this](SomeWorldComponent& owner, DropFilesUIEvent const& event) {
        for (auto& filePath : event.filePaths) {

            StandardResourceRepositoryModel repoModel;

            Document::Config documentConfig{
                .filePath = filePath,
                .name = fileManager->FileName(filePath, FileManager::FileNameFormat::NoExtension)
            };
            SP<Document> document = MAKE<Document>(documentConfig);
            ResourceRepository repo(repoModel, document->resources, *fileManager);

            document->saveFunc = [](auto& _document) {
                auto& document = *(static_cast<Document*>(&_document));

                PJ::SDLSurface surface({ .bitmap = document.core.bitmap.get() });
                IMG_SavePNG(surface.Surface(), document.GetFilePath().string().c_str());

                return Void{};
            };

            ResourceInfo info{ .id = filePath, .filePath = filePath, .type = ResourceType::Bitmap };
            repo.Load(info);

            auto bitmap = document->resources.TypeFirst<RGBABitmap>(ResourceType::Bitmap);
            if (bitmap) {
                document->core.bitmap = bitmap;
                document->core.texture =
                    MAKE<GLTexture>(GLTexture::Config{ .bitmap = bitmap.get() });

                documents.Add(document);
                documents.SetActiveDocument(document);
            }
        }
    };

    AddSignalHandler<DropFilesUIEvent>({ .id = SignalId::FilesDrop, .func = dropFilesHandler });

    AddSignalHandler<KeyDownUIEvent>({ .id = SignalId::KeyDown,
                                       .func = [this](auto& component, auto& keyDownEvent) {
                                           switch (keyDownEvent.KeyCodeValue()) {
                                           case SDLK_UP:
                                               documents.ActivatePreviousDocument(WrapType::Wrap);
                                               break;
                                           case SDLK_DOWN:
                                               documents.ActivateNextDocument(WrapType::Wrap);
                                               break;
                                           }
                                       } });
}

void ImageAppScene::LoadInto(WorldNode& root) {
    QB(root)
        .Named("Image App")
        .OrthoStandard()
        .With<ImRenderer>(ImRenderer::Config{ .worldSize = root.World()->Window()->PixelSize(),
                                              .areShapesOpaque = true })
        .SizeWithWindow()
        .ModifyLatest<ImRenderer>([this](auto& renderer) {
            renderer.AddSignalHandler(
                { .id = SignalId::RenderPrepare,
                  .func =
                      [this](auto& renderer, auto& signal) {
                          ImRenderer& im = *(static_cast<ImRenderer*>(&renderer));

                          im.SetForegroundColor(Color::gray);

                          Vector2 imSize = im.WorldSize();
                          im.FillRect({ .size = imSize });

                          SP<ImageAppScene::Document> activeDocument =
                              DCAST<ImageAppScene::Document>(documents.ActiveDocument());
                          GUARD(activeDocument)

                          auto texture = activeDocument.get()->core.texture;
                          GUARD(texture)

                          Vector2 textureSize = texture->Size();
                          float pad = 20;

                          Vector2 frameSize = { textureSize.x + pad * 2, textureSize.y + pad * 2 };
                          Rect frameBounds{ .size = frameSize };
                          frameBounds = CartesianUtils::CenteredIn(frameBounds, imSize);

                          auto shadowBounds = frameBounds;
                          shadowBounds.origin += { 10 * vecRight, 10 * vecDown };
                          im.FillRect(shadowBounds, Color::black);
                          im.FillRect(frameBounds, Color::white);

                          Rect textureBounds{ .size = textureSize };
                          textureBounds = CartesianUtils::CenteredIn(textureBounds, imSize);

                          im.Image(texture, textureBounds.origin);
                      } }
            );
        });
}

void ImageAppScene::Run(SP<Document> document, BitmapOperation& operation) {
    GUARD(document && document->core.bitmap)
    documents.SetActiveDocument(document);
    operation.Run(*document->core.bitmap);

    document->ModifyPush();
    document->core.texture =
        MAKE<GLTexture>(GLTexture::Config{ .bitmap = document->core.bitmap.get() });
}

void ImageAppScene::RegisterClasses() {
    commandClasses.Add(NEW<CommandClass>("flip.vertical", [this]() {
        return NEW<Command>(Command::Config{
            .name = "Flip vertical",
            .core = { .document = SCAST<Document>(this->documents.ActiveDocument()) },
            .runFunc =
                [this](auto& command) {
                    Run(command.core.document,
                        *BitmapOperations::NewFlipVertical(UpdateOrientationType::None));
                },
            .undoFunc =
                [this](auto& command) {
                    Run(command.core.document,
                        *BitmapOperations::NewFlipVertical(UpdateOrientationType::None));
                } });
    }));
    commandClasses.Add(NEW<CommandClass>("flip.horizontal", [this]() {
        return NEW<Command>(Command::Config{
            .name = "Flip horizontal",
            .core = { .document = SCAST<Document>(this->documents.ActiveDocument()) },
            .runFunc = [this](auto& command
                       ) { Run(command.core.document, *BitmapOperations::NewFlipHorizontal()); },
            .undoFunc = [this](auto& command
                        ) { Run(command.core.document, *BitmapOperations::NewFlipHorizontal()); } }
        );
    }));
    commandClasses.Add(NEW<CommandClass>("rotate.right", [this]() {
        return NEW<Command>(Command::Config{
            .name = "Rotate right",
            .core = { .document = SCAST<Document>(this->documents.ActiveDocument()) },
            .runFunc = [this](auto& command
                       ) { Run(command.core.document, *BitmapOperations::NewRotateRight()); },
            .undoFunc = [this](auto& command
                        ) { Run(command.core.document, *BitmapOperations::NewRotateLeft()); } });
    }));
    commandClasses.Add(NEW<CommandClass>("rotate.left", [this]() {
        return NEW<Command>(Command::Config{
            .name = "Rotate left",
            .core = { .document = SCAST<Document>(this->documents.ActiveDocument()) },
            .runFunc = [this](auto& command
                       ) { Run(command.core.document, *BitmapOperations::NewRotateLeft()); },
            .undoFunc = [this](auto& command
                        ) { Run(command.core.document, *BitmapOperations::NewRotateRight()); } });
    }));

    // Dither commands are lossy — runFunc snapshots the source pixels into a shared
    // buffer that undoFunc restores. The same buffer is re-snapshotted on redo.
    auto registerDither = [this](String id, String name, auto factoryFunc) {
        commandClasses.Add(NEW<CommandClass>(id, [this, name, factoryFunc]() {
            auto snapshot = MAKE<VectorList<RGBAColor>>();

            return NEW<Command>(Command::Config{
                .name = name,
                .core = { .document = SCAST<Document>(this->documents.ActiveDocument()) },
                .runFunc =
                    [this, snapshot, factoryFunc](auto& command) {
                        GUARD(command.core.document && command.core.document->core.bitmap)
                        auto data = command.core.document->core.bitmap->Data();
                        snapshot->assign(data.begin(), data.end());
                        Run(command.core.document, *factoryFunc());
                    },
                .undoFunc =
                    [this, snapshot](auto& command) {
                        GUARD(command.core.document && command.core.document->core.bitmap)
                        auto& bitmap = *command.core.document->core.bitmap;
                        auto data = bitmap.Data();
                        GUARD(data.size() == snapshot->size())
                        std::copy(snapshot->begin(), snapshot->end(), data.begin());

                        documents.SetActiveDocument(command.core.document);
                        command.core.document->ModifyPush();
                        command.core.document->core.texture =
                            MAKE<GLTexture>(GLTexture::Config{ .bitmap = &bitmap });
                    },
            });
        }));
    };

    registerDither("dither.floydSteinberg", "Floyd-Steinberg dither", []() {
        return BitmapOperations::NewFloydSteinbergDither();
    });
    registerDither("dither.atkinson", "Atkinson dither", []() {
        return BitmapOperations::NewAtkinsonDither();
    });
    registerDither("dither.bayer", "Bayer dither", []() {
        return BitmapOperations::NewBayerDither();
    });
}
