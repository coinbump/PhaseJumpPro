#include "PaintAppScene.h"
#include <SDL3/SDL_dialog.h>
#include <SDL3_image/SDL_image.h>

using namespace std;
using namespace PJ;

namespace {
    SDL_DialogFileFilter const pngFilters[] = { { .name = "PNG Image", .pattern = "png" } };
    int const pngFilterCount = sizeof(pngFilters) / sizeof(pngFilters[0]);

    /// Heap-owned context passed through SDL's C save-dialog callback. Scene is assumed to
    /// outlive the dialog — scenes live for the duration of the app in Kaiju.
    struct SaveDialogContext {
        PaintAppScene* scene{};
    };

    struct LoadDialogContext {
        PaintAppScene* scene{};
    };

    void OnSaveDialogResult(void* userdata, char const* const* filelist, int /*filter*/) {
        std::unique_ptr<SaveDialogContext> context(static_cast<SaveDialogContext*>(userdata));
        GUARD(context && context->scene)
        GUARD(filelist && filelist[0])

        String path = filelist[0];
        if (!path.ends_with(".png")) {
            path += ".png";
        }
        context->scene->WriteCanvasToPath(path);
    }

    void OnLoadDialogResult(void* userdata, char const* const* filelist, int /*filter*/) {
        std::unique_ptr<LoadDialogContext> context(static_cast<LoadDialogContext*>(userdata));
        GUARD(context && context->scene)
        GUARD(filelist && filelist[0])
        context->scene->LoadImageFromPath(filelist[0]);
    }
} // namespace

PaintAppScene::PaintAppScene() {
    // TODO: make platform-agnostic
    fileManager = NewMacFileManager();

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;
        planner.Button({ .label = "Save", .func = [this]() { SaveCanvas(); } });
        planner.Button({ .label = "Save As", .func = [this]() { SaveCanvasAs(); } });
        planner.Button({ .label = "Load", .func = [this]() { LoadImageDialog(); } });

        Binding<Color> brushColorBinding({ .getFunc =
                                               [this]() {
                                                   auto canvas = paintCanvas.lock();
                                                   return canvas ? canvas->brushColor
                                                                 : Color::black;
                                               },
                                           .setFunc =
                                               [this](auto& value) {
                                                   auto canvas = paintCanvas.lock();
                                                   GUARD(canvas)
                                                   canvas->brushColor = value;
                                               } });
        planner.PickerColor({ .label = "Brush Color", .binding = brushColorBinding });
    };
    Override(planUIFuncs[UIContextId::Editor], planUIFunc);

    auto dropFilesHandler = [this](SomeWorldComponent& /*owner*/, DropFilesUIEvent const& event) {
        for (auto& filePath : event.filePaths) {
            GUARD_CONTINUE(filePath.string().ends_with(".png"))
            LoadImageFromPath(filePath);
        }
    };
    AddSignalHandler<DropFilesUIEvent>({ .id = SignalId::FilesDrop, .func = dropFilesHandler });
}

void PaintAppScene::LoadInto(WorldNode& root) {
    SP<PaintCanvas> canvasPtr;

    QB(root)
        .Named("Paint App")
        .OrthoStandard()
        .With<PaintCanvas>(PaintCanvas::Config{ .pixelSize = Vector2(2000, 2000) })
        .ModifyLatest<PaintCanvas>([&canvasPtr](auto& canvas) {
            canvas.SetActiveTool(MAKE<BrushPaintTool>());
            canvasPtr = DCAST<PaintCanvas>(canvas.shared_from_this());
        });

    paintCanvas = canvasPtr;
}

void PaintAppScene::SaveCanvas() {
    auto activeDoc = DCAST<Document>(documents.ActiveDocument());
    if (activeDoc && !activeDoc->GetFilePath().empty()) {
        WriteCanvasToPath(activeDoc->GetFilePath());
        return;
    }
    SaveCanvasAs();
}

void PaintAppScene::SaveCanvasAs() {
    GUARD(!paintCanvas.expired())
    auto* context = new SaveDialogContext{ this };

    SDL_ShowSaveFileDialog(
        OnSaveDialogResult, context,
        /*window*/ nullptr, pngFilters, pngFilterCount,
        /*default_location*/ nullptr
    );
}

void PaintAppScene::WriteCanvasToPath(FilePath filePath) {
    auto canvas = paintCanvas.lock();
    GUARD(canvas)

    auto viewportNode = canvas->CanvasViewportNode();
    GUARD(viewportNode)

    auto viewport = viewportNode->TypeComponent<Viewport>();
    GUARD(viewport && viewport->renderContext)

    auto buffer = DCAST<GLTextureBuffer>(viewport->renderContext);
    GUARD_LOG(buffer, "Paint canvas viewport context is not a GLTextureBuffer — cannot save")

    auto bitmap = buffer->NewBitmap();
    GUARD_LOG(bitmap, "Paint canvas buffer produced no bitmap — cannot save")

    SDLSurface surface({ .bitmap = bitmap.get() });
    GUARD(surface.Surface())
    IMG_SavePNG(surface.Surface(), filePath.string().c_str());

    // First save creates a Document so subsequent Saves reuse the same file path. Save As
    // to a different path creates a new Document (Document's filePath has no setter).
    SP<RGBABitmap> sharedBitmap(std::move(bitmap));
    auto activeDoc = DCAST<Document>(documents.ActiveDocument());
    if (activeDoc && activeDoc->GetFilePath() == filePath) {
        activeDoc->core.bitmap = sharedBitmap;
    } else {
        Document::Config config{
            .name = fileManager->FileName(filePath, FileManager::FileNameFormat::NoExtension),
            .filePath = filePath,
        };
        auto document = MAKE<Document>(config);
        document->core.bitmap = sharedBitmap;
        documents.Add(document);
        documents.SetActiveDocument(document);
    }
}

void PaintAppScene::LoadImageDialog() {
    auto* context = new LoadDialogContext{ this };

    SDL_ShowOpenFileDialog(
        OnLoadDialogResult, context,
        /*window*/ nullptr, pngFilters, pngFilterCount,
        /*default_location*/ nullptr,
        /*allow_many*/ false
    );
}

void PaintAppScene::LoadImageFromPath(FilePath filePath) {
    auto canvas = paintCanvas.lock();
    GUARD(canvas)

    // Load the bitmap via the resource repository — mirrors ImageAppScene's flow.
    StandardResourceRepositoryModel repoModel;
    Document::Config config{
        .name = fileManager->FileName(filePath, FileManager::FileNameFormat::NoExtension),
        .filePath = filePath,
    };
    SP<Document> document = MAKE<Document>(config);
    ResourceRepository repository(repoModel, document->resources, *fileManager);

    ResourceInfo info{ .id = filePath, .filePath = filePath, .type = ResourceType::Bitmap };
    repository.Load(info);

    auto bitmap = document->resources.TypeFirst<RGBABitmap>(ResourceType::Bitmap);
    GUARD_LOG(bitmap, "Failed to load bitmap from dropped/opened file")

    auto texture = MAKE<GLTexture>(GLTexture::Config{ .bitmap = bitmap.get() });
    GUARD(texture)

    document->core.bitmap = bitmap;
    documents.Add(document);
    documents.SetActiveDocument(document);

    canvas->SwapCanvas(texture);
}
