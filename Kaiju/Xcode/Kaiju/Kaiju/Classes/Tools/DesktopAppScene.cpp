#include "DesktopAppScene.h"
#include "ExampleCheckersScene.h"

using namespace std;
using namespace PJ;

namespace {
    using WindowConfig = Desktop::WindowConfig;

    WindowConfig MakeSampleWindowConfig(Vector2 origin) {
        return { .origin = origin,
                 .worldSize = { 600, 300 },
                 .buildContentFunc = [](WorldNode& contentNode) {
                     auto viewport = contentNode.TypeComponent<Viewport>();
                     GUARD(viewport)

                     auto viewportSize = viewport->worldSize.Value();
                     GUARD(viewportSize.x > 0 && viewportSize.y > 0)

                     auto& child = contentNode.AddNode(WorldNode::Config{ .name = "Content" });

                     auto& im = child.AddComponent<ImRenderer>(ImRenderer::Config{
                         .worldSize = viewportSize, .areShapesOpaque = true });

                     im.AddSignalHandler({ .id = SignalId::RenderPrepare,
                                           .func = [](auto& renderer, auto& signal) {
                                               auto& im = *static_cast<ImRenderer*>(&renderer);
                                               auto worldSize = im.WorldSize();

                                               im.FillRect({ .size = worldSize }, Color::blue);

                                               float radius =
                                                   std::min(worldSize.x, worldSize.y) * 0.4f;
                                               im.FillCircle(
                                                   { worldSize.x / 2.0f, 0 }, radius, Color::red
                                               );
                                           } });
                 } };
    }
} // namespace

DesktopAppScene::DesktopAppScene() {
    fileManager = NewMacFileManager();

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        planner.Button({ .label = "New Window", .func = [this]() { NewWindow(); } });
        planner.Button({ .label = "Checkers", .func = [this]() { NewCheckersWindow(); } });

        auto node = this->Node();
        GUARD(node && node->World())
        auto editorSystem = node->World()->TypeSystem<EditorWorldSystem>();
        GUARD(editorSystem)

        planner.ListSelect([this, editorSystem]() {
            auto namedClasses = MAKE<VectorList<SceneClass*>>();
            VectorList<String> options;
            for (auto& entry : editorSystem->sceneClasses.Map()) {
                auto& sceneClass = entry.second;
                GUARD_CONTINUE(sceneClass)
                auto& name = sceneClass->_core.name;
                GUARD_CONTINUE(!name.empty())
                namedClasses->push_back(sceneClass.get());
                options.push_back(name);
            }

            Binding<int> binding({ .getFunc = []() { return -1; },
                                   .setFunc =
                                       [this, namedClasses](auto& value) {
                                           GUARD(value >= 0 && value < (int)namedClasses->size())
                                           NewSceneWindow((*namedClasses)[value]);
                                       } });

            return UIPlanner::ListSelectConfig{ .label = "Scenes",
                                                .options = options,
                                                .binding = binding };
        });
    };

    Override(planUIFuncs[UIContextId::Editor], planUIFunc);

    AddSignalHandler<DropFilesUIEvent>(
        { .id = SignalId::FilesDrop,
          .func =
              [this](SomeWorldComponent& owner, DropFilesUIEvent const& event) {
                  for (auto& filePath : event.filePaths) {
                      GUARD_CONTINUE(fileManager)
                      auto extension = fileManager->FileExtension(
                          filePath, FileManager::FileExtensionFormat::NoDot
                      );
                      std::transform(
                          extension.begin(), extension.end(), extension.begin(),
                          [](unsigned char ch) { return (char)std::tolower(ch); }
                      );

                      auto origin = ScreenPositionToReadingOrigin(event.position);

                      if (extension == "wav") {
                          NewAudioWindow(filePath, origin);
                      } else if (extension == "png") {
                          NewImageWindow(filePath, origin);
                      }
                  }
              } }
    );
}

void DesktopAppScene::LoadInto(WorldNode& root) {
    QB(root).Named("Desktop").OrthoStandard();

    auto& desktopNode = root.AddNode(WorldNode::Config{ .name = "Desktop" });
    QB(desktopNode).With<Desktop>().SizeWithWindow();

    auto desktopPtr = desktopNode.TypeComponentPtr<Desktop>();
    desktop = desktopPtr;
    GUARD(desktopPtr)

    auto firstConfig = MakeSampleWindowConfig({ 50, 50 });
    firstConfig.windowLevel = NEW<FloatingWindowLevel>();
    firstConfig.windowStyle = NEW<PlainWindowStyle>();
    desktopPtr->NewWindow(firstConfig);
}

void DesktopAppScene::NewWindow() {
    auto desktopLock = desktop.lock();
    GUARD(desktopLock)

    int existingCount = (int)desktopLock->Windows().size();
    float offset = 30.0f * existingCount;

    desktopLock->NewWindow(MakeSampleWindowConfig({ 50 + offset, 50 + offset }));
}

void DesktopAppScene::NewCheckersWindow() {
    auto desktopLock = desktop.lock();
    GUARD(desktopLock)

    int existingCount = (int)desktopLock->Windows().size();
    float offset = 30.0f * existingCount;

    desktopLock->NewWindow({ .origin = { 50 + offset, 50 + offset },
                             .worldSize = { 600, 600 },
                             .buildContentFunc = [](WorldNode& contentNode) {
                                 contentNode.AddComponent<Example::Checkers::Scene>();
                             } });
}

void DesktopAppScene::NewSceneWindow(SceneClass* sceneClass) {
    GUARD(sceneClass)
    auto scene = sceneClass->Make();
    GUARD(scene)

    auto desktopLock = desktop.lock();
    GUARD(desktopLock)

    int existingCount = (int)desktopLock->Windows().size();
    float offset = 30.0f * existingCount;

    desktopLock->NewWindow({ .origin = { 50 + offset, 50 + offset },
                             .worldSize = { 1200, 1200 },
                             .buildContentFunc = [scene](WorldNode& contentNode) {
                                 contentNode.Add(scene);
                             } });
}

Vector2 DesktopAppScene::ScreenPositionToReadingOrigin(Vector2 screenPosition) const {
    auto desktopLock = desktop.lock();
    GUARDR(desktopLock, screenPosition)
    auto desktopNode = desktopLock->Node();
    GUARDR(desktopNode, screenPosition)
    auto world = desktopNode->World();
    GUARDR(world, screenPosition)
    auto camera = world->MainCamera();
    GUARDR(camera, screenPosition)

    // Screen → world (cartesian, +y up, in world units — handles DPI scaling).
    auto worldPosition = camera->ScreenToContext(screenPosition);

    // World → desktop-local (cartesian, centered, +y up).
    auto desktopLocal = desktopNode->WorldToLocal(worldPosition);

    // Desktop-local cartesian → desktop reading coords (top-left origin, +y down).
    auto desktopSize = desktopLock->worldSize.Value();
    auto halfDesktop = desktopSize * 0.5f;
    return Vector2(desktopLocal.x + halfDesktop.x, halfDesktop.y - desktopLocal.y);
}

void DesktopAppScene::NewAudioWindow(FilePath filePath, Vector2 origin) {
    auto desktopLock = desktop.lock();
    GUARD(desktopLock)

    StandardResourceRepositoryModel repoModel;
    SDLLoadAudioStreamOperation operation(
        ResourceInfo{ .filePath = filePath, .type = "audio" }, repoModel
    );
    operation.Run();

    auto resourceCatalog = operation.Resources();
    GUARD(!IsEmpty(resourceCatalog))
    auto stream = SCAST<SDLAudioStream>(resourceCatalog[0].resource);
    GUARD(stream)

    desktopLock->NewWindow({ .origin = origin,
                             .worldSize = { 900, 300 },
                             .buildContentFunc = [stream](WorldNode& contentNode) {
                                 auto viewport = contentNode.TypeComponent<Viewport>();
                                 GUARD(viewport)
                                 auto viewportSize = viewport->worldSize.Value();
                                 GUARD(viewportSize.x > 0 && viewportSize.y > 0)

                                 auto& child =
                                     contentNode.AddNode(WorldNode::Config{ .name = "Waveform" });
                                 auto& renderer = child.AddComponent<SDLAudioStreamRenderer>(
                                     SDLAudioStreamRenderer::Config{ .worldSize = viewportSize,
                                                                     .maxSegmentsCount = 2000,
                                                                     .color = Color::red }
                                 );
                                 renderer.SetAudioStream(stream);
                             } });
}

void DesktopAppScene::NewImageWindow(FilePath filePath, Vector2 origin) {
    auto desktopLock = desktop.lock();
    GUARD(desktopLock)

    ResourceRepositoryModel repoModel;
    ResourceInfo info{ .filePath = filePath, .type = ResourceType::Texture };
    auto loadTexture = MAKE<SDLLoadGLTextureOperation>(info, repoModel);
    loadTexture->Run();

    auto successValue = loadTexture->result->SuccessValue();
    GUARD(successValue)
    GUARD(!IsEmpty(successValue->resources))
    auto texture = DCAST<Texture>(successValue->resources[0].resource);
    GUARD(texture)

    desktopLock->NewWindow({ .origin = origin,
                             .worldSize = { 600, 600 },
                             .buildContentFunc = [texture](WorldNode& contentNode) {
                                 auto viewport = contentNode.TypeComponent<Viewport>();
                                 GUARD(viewport)
                                 auto viewportSize = viewport->worldSize.Value();
                                 GUARD(viewportSize.x > 0 && viewportSize.y > 0)

                                 auto& child =
                                     contentNode.AddNode(WorldNode::Config{ .name = "Image" });
                                 auto& renderer = child.AddComponent<SpriteRenderer>(
                                     SpriteRenderer::Config{ .texture = texture }
                                 );
                                 renderer.SetWorldSize(Vector3(viewportSize.x, viewportSize.y, 0));
                             } });
}
