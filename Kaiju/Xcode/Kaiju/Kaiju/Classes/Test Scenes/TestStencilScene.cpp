#include "TestStencilScene.h"

using namespace std;
using namespace PJ;

namespace {
    constexpr float squareSize = 2000.0f;
    constexpr float circleRadius = 600.0f;

    class StencilCircleRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = StencilCircleRenderer;

        Vector2 size;
        SP<RenderMaterial> stencilMaterial;

        StencilCircleRenderer(Vector2 size) :
            size(size) {}

        // MARK: Renderer

        RenderModelList RenderModels() override {
            if (!stencilMaterial) {
                stencilMaterial = ColorRenderer::MakeMaterial(RenderOpacityType::Opaque);
            }

            auto pushRenderModel = MAKE<StencilPushRenderModel>();
            pushRenderModel->mesh = EllipseMeshBuilder(size).BuildMesh();
            pushRenderModel->material = stencilMaterial.get();
            if (owner) {
                pushRenderModel->matrix = owner->ModelMatrix();
            }
            RenderModelList result;
            result.push_back(pushRenderModel);
            return result;
        }

        RenderModelList PostRenderModels() override {
            auto pop = MAKE<StencilPopRenderModel>();
            RenderModelList result;
            result.push_back(pop);
            return result;
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return Vector3(size.x, size.y, 0);
        }

        void SetWorldSize(Vector3 value) override {
            size = Vector2(value.x, value.y);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "StencilCircleRenderer";
        }
    };
} // namespace

void TestStencilScene::LoadInto(WorldNode& root) {
    QB(root).Named("Test Stencil").OrthoStandard().SizeWithWindow();

    // Parent node owns the stencil pushRenderModel/pop. Its render model wraps the child's draw in
    // the DAG: parent.RenderModels() (StencilPush) sits as parent, child.material attaches
    // under it, parent.PostRenderModels() (StencilPop) is added as a sibling-after.
    auto& parentNode = root.And(WorldNode::Config{ .name = "StencilParent" });
    parentNode.AddComponent<StencilCircleRenderer>(Vector2(circleRadius * 2, circleRadius * 2));

    auto& childNode = parentNode.And(WorldNode::Config{ .name = "ChildSquare" });
    childNode.AddComponent<ColorRenderer>(ColorRenderer::Config{
        .color = Color::red, .worldSize = { squareSize, squareSize } });
}
