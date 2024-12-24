#include "ImageAppScene.h"

using namespace std;
using namespace PJ;

ImageAppScene::ImageAppScene() {
    Document::DocumentFunc onCloseFunc = [this](auto& document) {
        SetActiveDocument(documents.Count() > 0 ? documents[0] : nullptr);
    };
    Override(documents.onCloseFunc, onCloseFunc);

    PlanUIFunc planUIFunc = [this](auto& component, String context, UIPlanner& planner) {
        {
            planner.ListSelect([this]() {
                VectorList<String> options = Map<String>(documents.Documents(), [](auto& document) {
                    return document->name;
                });

                Binding<int> binding{ [this]() {
                                         auto result =
                                             IndexOf(documents.Documents(), activeDocument);
                                         return result ? *result : -1;
                                     },
                                      [=, this](auto& value) {
                                          GUARD(value >= 0 && value < documents.Count())
                                          auto document = documents.Documents()[value];
                                          SetActiveDocument(document);
                                          ;
                                      } };

                return UIPlanner::ListSelectConfig{ .label = "Documents",
                                                    .options = options,
                                                    .binding = binding };
            });
        }

        {
            VectorList<BitmapOperationClass*> operationClasses;
            for (auto& operationClass : this->operationClasses.Map()) {
                operationClasses.push_back(operationClass.second.get());
            }

            VectorList<String> options =
                Map<String>(operationClasses, [](auto& _class) { return _class->core.name; });

            Binding<int> binding{ [this]() { return -1; },
                                  [=, this](auto& value) {
                                      GUARD(value >= 0 && value < operationClasses.size())
                                      // TODO: operationClasses[value].NewType(
                                      ;
                                  } };

            planner.ListSelect({ .label = "Operations", .options = options, .binding = binding });
        }

        planner.Button({ .label = "Save", .func = [this]() {
                            GUARD(activeDocument);
                            activeDocument->Save();
                        } });

        planner.Button({ .label = "Close", .func = [this]() {
                            GUARD(activeDocument);
                            activeDocument->Close();
                        } });
    };

    Override(planUIFuncs[UIContextId::Editor], planUIFunc);

    RegisterOperationClasses();

    auto dropFilesHandler = [this](SomeWorldComponent& owner, DropFilesUIEvent const& event) {
        for (auto& filePath : event.filePaths) {

            StandardResourceRepositoryModel repoModel;
            SDLFileManager fm;

            Document::Config documentConfig{ .filePath = filePath,
                                             .name = fm.FileName(filePath, false) };
            SP<Document> document = MAKE<Document>(documentConfig);
            ResourceRepository repo(repoModel, document->resources, fm);

            ResourceInfo info{ .id = filePath,
                               .filePath = filePath,
                               .type = ResourceType::Texture };
            repo.Load(info);

            documents.Add(document);
            SetActiveDocument(document);
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

            SetActiveDocument(documents[indexValue]);
            return;
        }
    }

    SetActiveDocument(documents[0]);
}

void ImageAppScene::NavigateToNextDocument() {
    GUARD(documents.Count() > 0)

    if (activeDocument) {
        auto index = IndexOf(documents.Documents(), activeDocument);
        if (index) {
            int indexValue = (int)*index;
            indexValue++;
            indexValue %= (int)documents.Count();

            SetActiveDocument(documents[indexValue]);
            return;
        }
    }

    SetActiveDocument(documents[0]);
}

void ImageAppScene::LoadInto(WorldNode& root) {
    RegisterOperationClasses();

    QB(root)
        .Named("Image App")
        .OrthoStandard()
        .With<ImRenderer>(ImRenderer::Config{ .worldSize = { 1400, 1400 }, .areShapesOpaque = true }
        )
        .ModifyLatest<ImRenderer>([this](auto& renderer) {
            renderer.AddSignalHandler({ .id = SignalId::RenderPrepare,
                                        .func = [this](auto& renderer, auto& signal) {
                                            ImRenderer& im = *(static_cast<ImRenderer*>(&renderer));

                                            im.SetColor(Color::gray);

                                            Vector2 imSize = im.WorldSize();
                                            im.FillRect({ .size = imSize });

                                            GUARD(activeDocument)

                                            auto texture = activeDocument->resources.FindTexture(
                                                activeDocument->GetFilePath().string()
                                            );
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
    operationClasses.Add(NEW<BitmapOperationClass>(
        "flipV", "Flip vertical",
        [](BitmapOperations::BitmapType& bitmap) { return BitmapOperations::FlipV(bitmap); }
    ));
}
