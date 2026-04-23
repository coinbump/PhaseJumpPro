#include "TestNestedViewportScene.h"

using namespace std;
using namespace PJ;

void TestNestedViewportScene::LoadInto(WorldNode& root) {
    // Color renderers live on child nodes of each viewport so they render into the viewport's
    // buffer. Components on the viewport-owner node itself would render via the outer camera
    // (the viewport's owner lives in the outer world per RenderWorldSystem's convention).
    SP<Texture> parentTexture;
    SP<Texture> childTexture;

    QB(root)
        .Named("Test: Nested viewport")
        .OrthoStandard()
        .And("Parent Viewport")
        .With<Viewport>()
        .ModifyLatest<Viewport>([&parentTexture](auto& viewport) {
            viewport.worldSize = Vector2(800, 800);

            // Capture the viewport's backing texture so the top-level sprite can display it.
            // Setting worldSize triggers ResizeBuffer, which populates `renderContext`.
            if (viewport.renderContext) {
                parentTexture = viewport.renderContext->GetTexture();
            }
        })
        .And("Parent Color")
        .With<ColorRenderer>(ColorRenderer::Config{ .color = Color::blue,
                                                    .worldSize = Vector2(800, 800) })
        .Pop()
        .And("Inner Viewport")
        .With<Viewport>()
        .ModifyLatest<Viewport>([](auto& viewport) { viewport.worldSize = Vector2(300, 300); })
        .ModifyLatest<Viewport>([&childTexture](auto& viewport) {
            viewport.worldSize = Vector2(300, 300);

            // Capture the viewport's backing texture so the top-level sprite can display it.
            // Setting worldSize triggers ResizeBuffer, which populates `renderContext`.
            if (viewport.renderContext) {
                childTexture = viewport.renderContext->GetTexture();
            }
        })
        .And("Inner Color")
        .With<ColorRenderer>(ColorRenderer::Config{ .color = Color::red,
                                                    .worldSize = Vector2(300, 300) });

    // Top-level composite sprite: sibling of Parent Viewport under root. Rendered by the main
    // camera, not by any viewport, so it displays Parent Viewport's buffer on-screen.
    QB(root)
        .And("Parent Composite")
        .With<SpriteRenderer>(SpriteRenderer::Config{ .texture = parentTexture })
        .ModifyLatest<SpriteRenderer>([](auto& sprite) {
            sprite.SetWorldSize(Vector3(800, 800, 0));
        })
        .And("Child Composite")
        .With<SpriteRenderer>(SpriteRenderer::Config{ .texture = childTexture })
        .ModifyLatest<SpriteRenderer>([](auto& sprite) {
            sprite.SetWorldSize(Vector3(300, 200, 0));
        });
}
