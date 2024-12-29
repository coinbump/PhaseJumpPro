#include "ImageAppScene.h"

using namespace std;
using namespace PJ;

ImageAppScene::ImageAppScene() {
    RegisterOperationClasses();

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        {
            planner.ListSelect([this]() {
                VectorList<String> options = Map<String>(documents.Documents(), [](auto& document) {
                    return document->name;
                });

                Binding<int> binding{
                    [this]() {
                        auto result = IndexOfIf(documents.Documents(), [this](auto& document) {
                            return document.get() == activeDocument.get();
                        });
                        return result ? *result : -1;
                    },
                    [=, this](auto& value) {
                        GUARD(value >= 0 && value < documents.Count())
                        auto document = SCAST<Document>(documents.Documents()[value]);
                        SetActiveDocument(document);
                    }
                };

                return UIPlanner::ListSelectConfig{ .label = "Documents",
                                                    .options = options,
                                                    .binding = binding };
            });
        }

        planner.ListSelect([this]() {
            VectorList<BitmapOperationClass*> operationClasses;
            for (auto& operationClass : this->operationClasses.Map()) {
                operationClasses.push_back(operationClass.second.get());
            }

            VectorList<String> options =
                Map<String>(operationClasses, [](auto& _class) { return _class->core.name; });

            Binding<int> binding{ [this]() { return -1; },
                                  [=, this](auto& value) {
                                      GUARD(activeDocument && activeDocument->core.bitmap)
                                      GUARD(value >= 0 && value < operationClasses.size())

                                      auto bitmap = activeDocument->core.bitmap;

                                      auto _class = operationClasses[value];
                                      auto operation = _class->New();
                                      GUARD(operation)

                                      //                                      BitmapOperationCommandCore
                                      //                                      commandCore{ .document
                                      //                                      = activeDocument,
                                      //                                      .bitmap =
                                      //                                      bitmap->Clone(),
                                      //                                      .operation = operation
                                      //                                      }; UP<SomeCommand>
                                      //                                      command =
                                      //                                      NEW<Command<BitmapOperationCommandCore>>(
                                      //                                          _class->Core.name,
                                      //                                          [](auto& command)
                                      //                                          {
                                      //                                          command.core.operation(*command.core.bitmap);
                                      //                                          },
                                      //                                          [](auto& command)
                                      //                                          {
                                      //                                              command.core.document->bitmap
                                      //                                              =
                                      //                                              command.core.bitmap
                                      //                                          }
                                      //                                      );
                                      //                                      commands.Run(command);

                                      operation->Run(*bitmap);
                                      activeDocument->core.texture =
                                          MAKE<GLTexture>(GLTexture::Config{
                                              .bitmap = activeDocument->core.bitmap.get() });
                                  } };

            return UIPlanner::ListSelectConfig{ .label = "Operations",
                                                .options = options,
                                                .binding = binding };
        });

        planner.Button({ .label = "Save", .func = [this]() {
                            GUARD(activeDocument);
                            activeDocument->Save();
                        } });

        planner.Button({ .label = "Close", .func = [this]() {
                            GUARD(activeDocument);
                            documents.Remove(*activeDocument);
                            SetActiveDocument(
                                documents.Count() > 0 ? SCAST<Document>(documents[0]) : nullptr
                            );
                        } });
    };

    Override(planUIFuncs[UIContextId::Editor], planUIFunc);

    auto dropFilesHandler = [this](SomeWorldComponent& owner, DropFilesUIEvent const& event) {
        for (auto& filePath : event.filePaths) {

            StandardResourceRepositoryModel repoModel;
            SDLFileManager fm;

            Document::Config documentConfig{ .filePath = filePath,
                                             .name = fm.FileName(filePath, false) };
            SP<Document> document = MAKE<Document>(documentConfig);
            ResourceRepository repo(repoModel, document->resources, fm);

            document->saveFunc = [](auto& document) {
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
                SetActiveDocument(document);
            }
        }
    };

    AddSignalHandler<DropFilesUIEvent>({ .id = SignalId::DropFiles, .func = dropFilesHandler });

    AddSignalHandler<KeyDownUIEvent>({ .id = SignalId::KeyDown,
                                       .func = [this](auto& component, auto& keyDownEvent) {
                                           switch (keyDownEvent.keyCode.value) {
                                           case SDLK_UP:
                                               NavigateToPreviousDocument();
                                               break;
                                           case SDLK_DOWN:
                                               NavigateToNextDocument();
                                               break;
                                           }
                                       } });
}

void ImageAppScene::NavigateToPreviousDocument() {
    GUARD(documents.Count() > 0)

    if (activeDocument) {
        auto index = IndexOf(documents.Documents(), activeDocument);
        if (index) {
            int indexValue = (int)*index;
            indexValue--;
            if (indexValue < 0) {
                // Can't use unsigned Count value for logic
                indexValue = (int)documents.Count() - 1;
            }

            SetActiveDocument(SCAST<Document>(documents[indexValue]));
            return;
        }
    }

    SetActiveDocument(SCAST<Document>(documents[0]));
}

void ImageAppScene::NavigateToNextDocument() {
    GUARD(documents.Count() > 0)

    if (activeDocument) {
        auto index = IndexOf(documents.Documents(), activeDocument);
        if (index) {
            int indexValue = (int)*index;
            indexValue++;
            indexValue %= (int)documents.Count();

            SetActiveDocument(SCAST<Document>(documents[indexValue]));
            return;
        }
    }

    SetActiveDocument(SCAST<Document>(documents[0]));
}

void ImageAppScene::LoadInto(WorldNode& root) {
    RegisterOperationClasses();

    QB(root)
        .Named("Image App")
        .OrthoStandard()
        .With<ImRenderer>(ImRenderer::Config{ .worldSize = root.World()->Window()->PixelSize(),
                                              .areShapesOpaque = true })
        .SizeWithWindow()
        .ModifyLatest<ImRenderer>([this](auto& renderer) {
            renderer.AddSignalHandler({ .id = SignalId::RenderPrepare,
                                        .func = [this](auto& renderer, auto& signal) {
                                            ImRenderer& im = *(static_cast<ImRenderer*>(&renderer));

                                            im.SetColor(Color::gray);

                                            Vector2 imSize = im.WorldSize();
                                            im.FillRect({ .size = imSize });

                                            GUARD(activeDocument)

                                            auto texture = activeDocument->core.texture;
                                            GUARD(texture)

                                            Vector2 textureSize = texture->Size();
                                            float pad = 20;

                                            Vector2 frameSize = { textureSize.x + pad * 2,
                                                                  textureSize.y + pad * 2 };
                                            Rect frameBounds{ .size = frameSize };
                                            frameBounds =
                                                CartesianUtils::CenteredIn(frameBounds, imSize);

                                            auto shadowBounds = frameBounds;
                                            shadowBounds.origin += { 10 * vecRight, 10 * vecDown };
                                            im.FillRect(shadowBounds, Color::black);
                                            im.FillRect(frameBounds, Color::white);

                                            Rect textureBounds{ .size = textureSize };
                                            textureBounds =
                                                CartesianUtils::CenteredIn(textureBounds, imSize);

                                            im.Image(texture, textureBounds.origin);
                                        } });
        });
}

void ImageAppScene::RegisterOperationClasses() {
    operationClasses.Add(NEW<BitmapOperationClass>("flipV", "Flip vertical", []() {
        return BitmapOperations::FlipV();
    }));

    PJ::Log("Bitmap operations are registered");
}
