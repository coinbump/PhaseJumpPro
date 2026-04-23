#include "TestImClipScene.h"

using namespace std;
using namespace PJ;

void TestImClipScene::LoadInto(WorldNode& root) {
    QB(root)
        .Named("TestImClipScene")
        .OrthoStandard()
        .With<ImRenderer>(ImRenderer::Config{ .worldSize = root.World()->Window()->PixelSize() })
        .SizeWithWindow()
        .ModifyLatest<ImRenderer>([](auto& renderer) {
            renderer.AddSignalHandler({ .id = SignalId::RenderPrepare,
                                        .func = [](auto& renderer, auto& signal) {
                                            ImRenderer& im = *(static_cast<ImRenderer*>(&renderer));

                                            Vector2 imSize = im.WorldSize();
                                            Vector2 clipSize = { imSize.x * 0.5f, imSize.y * 0.5f };
                                            Vector2 clipOrigin = { (imSize.x - clipSize.x) * 0.5f,
                                                                   (imSize.y - clipSize.y) * 0.5f };
                                            Rect clipFrame{ .origin = clipOrigin,
                                                            .size = clipSize };

                                            im.FillRect(
                                                { .origin = {}, .size = imSize }, Color::blue
                                            );
                                            im.ClipPush(clipFrame, NEW<EllipseShape>());
                                            im.FillRect(clipFrame, Color::red);
                                            im.ClipPop();
                                        } });
        });
}
