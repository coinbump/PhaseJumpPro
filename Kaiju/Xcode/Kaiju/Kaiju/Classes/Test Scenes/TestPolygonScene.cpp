#include "TestPolygonScene.h"

using namespace std;
using namespace PJ;

namespace {
    auto constexpr polygonNodeId = "polygon";
} // namespace

TestPolygonScene::TestPolygonScene() {
    AddSignalHandler<PointerDownUIEvent>({ .id = SignalId::PointerDown,
                                           .func = [](auto& component, auto& event) {
                                               static_cast<This*>(&component)->OnPointerDown(event);
                                           } });

    uiSystemResolver = [this]() -> UIWorldSystem* {
        GUARDR(owner, {})

        auto const world = owner->World();
        GUARDR(world, {})

        return world->TypeSystem<UIWorldSystem>();
    };
}

void TestPolygonScene::OnPointerDown(PointerDownUIEvent const& event) {
    GUARD(owner)

    auto inputWorldPosition = ScreenToWorld(*this, event.core.screenPos);
    auto inputLocalPosition = WorldToLocal(inputWorldPosition);

    polygon.Add(inputLocalPosition);

    PolygonMeshBuilder builder;

    auto textNode = (*owner)[polygonNodeId];
    if (textNode) {
        auto colorRenderer = textNode->TypeComponent<ColorRenderer>();
        if (colorRenderer) {
            colorRenderer->SetWorldSize({ polygon.Width(), polygon.Height(), 0 });
            colorRenderer->core.model.SetMeshNeedsBuild();
        }
    }
}

void TestPolygonScene::LoadInto(WorldNode& root) {
    QB(root)
        .Named("Test: Polygon mesh")
        .OrthoStandard()
        .RectCollider({ 10000, 10000 })
        .And(polygonNodeId)
        .With<ColorRenderer>(ColorRenderer::Config{ .color = Color::red })
        .ModifyLatest<ColorRenderer>([this](auto& colorRenderer) {
            colorRenderer.core.SetBuildMeshFunc([this](auto& model) {
                PolygonMeshBuilder meshBuilder(polygon);
                return meshBuilder.BuildMesh();
            });
        });
}
