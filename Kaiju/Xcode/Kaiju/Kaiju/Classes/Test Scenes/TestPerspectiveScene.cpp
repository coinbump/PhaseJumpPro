#include "TestPerspectiveScene.h"

using namespace std;
using namespace PJ;

TestPerspectiveScene::TestPerspectiveScene() {
    faceColors = {
        Color::red,        // +Z front
        Color::green,      // +X right
        Color::blue,       // -Z back
        Color::yellow,     // -X left
        Color(1, 0, 1, 1), // +Y top (magenta)
        Color(0, 1, 1, 1), // -Y bottom (cyan)
    };

    PlanUIFunc planUIFunc = [this](auto args) {
        args.planner.Button({ .label = "New Cube", .func = [this]() { AddRandomCube(); } });
    };
    Override(planUIFuncs[UIContextId::Editor], planUIFunc);
}

void TestPerspectiveScene::LoadInto(WorldNode& root) {
    root.SetName("TestPerspectiveScene");

    auto pixelSize = root.World()->PixelSize();
    windowPixelSize = { (float)pixelSize.x, (float)pixelSize.y };
    cameraDistance = std::max(windowPixelSize.x, windowPixelSize.y);

    Vector3 const cubeSize(
        windowPixelSize.x * 0.35f, windowPixelSize.y * 0.35f, windowPixelSize.x * 0.35f
    );

    QB(root)
        .And("Camera")
        .PerspectiveStandard(QuickBuilder::PerspectiveStandardConfig{ .clearColor = Color::gray })
        .SetLocalPosition(Vector3(0, 0, cameraDistance))
        .Pop();

    AddCube(Vector3(0, 0, 0), cubeSize);
}

void TestPerspectiveScene::AddCube(Vector3 position, Vector3 size) {
    GUARD(owner)

    auto const faceColors = this->faceColors;
    VectorList<Color> vertexColors;
    vertexColors.reserve(24);
    for (auto const& faceColor : faceColors) {
        for (int i = 0; i < 4; i++) {
            vertexColors.push_back(faceColor);
        }
    }

    QB(*owner)
        .And("Cube")
        .With<ColorRenderer>(ColorRenderer::Config{ .color = vertexColors[0],
                                                    .worldSize = Vector2(size.x, size.y) })
        .ModifyLatest<ColorRenderer>([=](ColorRenderer& renderer) {
            renderer.SetWorldSize(size);
            renderer.core.SetBuildMeshFunc([](RendererModel const& model) {
                return BoxMeshBuilder(model.WorldSize()).BuildMesh();
            });
            renderer.core.model.SetBuildVertexColorsFunc([](RendererModel const& model,
                                                            VectorList<RenderColor>& out) {
                out.clear();
                for (auto const& c : model.Colors()) {
                    out.push_back(c);
                }
            });
            renderer.core.model.SetColors(vertexColors);

            // TODO: there is a bug with opaque 3D shapes, because of the batch renderer
            renderer.EnableBlend(true);
        })
        .ModifyNode([=](WorldNode& node) {
            node.SetLocalPosition(position);
            // Tilt so multiple faces are visible
            node.transform.SetRotation(Vector3(-25, 35, 0));
        })
        .With<WorldComponent>()
        .ModifyLatest<WorldComponent>([](WorldComponent& component) {
            component.GetUpdatables().AddContinue([&component](TimeSlice time) {
                auto node = component.Node();
                GUARD(node)
                Vector3 rotation = node->transform.Rotation();
                rotation.x += 20.0f * time.delta;
                rotation.y += 45.0f * time.delta;
                node->transform.SetRotation(rotation);
            });
        });
}

void TestPerspectiveScene::AddRandomCube() {
    GUARD(owner && owner->World())

    // Pick z somewhere in the visible depth range, in front of the camera.
    // Camera is at +Z looking toward -Z, so visible world-z lies below cameraDistance
    float const minZ = -cameraDistance * 0.25f;
    float const maxZ = cameraDistance * 0.6f;
    float const z = minZ + random.Value() * (maxZ - minZ);

    // At world-space z, distance from camera is (cameraDistance - z)
    float const distance = std::max(1.0f, cameraDistance - z);

    // PerspectiveStandard uses 60° vertical fov
    float const fovRadians = Angle::WithDegrees(60).Radians();
    float const aspect = (windowPixelSize.y > 0) ? (windowPixelSize.x / windowPixelSize.y) : 1.0f;
    float const visibleHalfHeight = distance * std::tan(fovRadians / 2.0f);
    float const visibleHalfWidth = visibleHalfHeight * aspect;

    // Random size between 10% and 25% of the shorter visible extent at this distance
    float const minVisible = std::min(visibleHalfWidth, visibleHalfHeight) * 2.0f;
    float const sizeMin = minVisible * 0.1f;
    float const sizeMax = minVisible * 0.25f;
    auto randomSize = [&]() { return sizeMin + random.Value() * (sizeMax - sizeMin); };
    Vector3 const size(randomSize(), randomSize(), randomSize());

    // Leave room so the box stays on screen after perspective projection
    float const xHalfRange = std::max(0.0f, visibleHalfWidth - size.x * 0.5f);
    float const yHalfRange = std::max(0.0f, visibleHalfHeight - size.y * 0.5f);
    float const x = -xHalfRange + random.Value() * xHalfRange * 2.0f;
    float const y = -yHalfRange + random.Value() * yHalfRange * 2.0f;

    AddCube(Vector3(x, y, z), size);
}
