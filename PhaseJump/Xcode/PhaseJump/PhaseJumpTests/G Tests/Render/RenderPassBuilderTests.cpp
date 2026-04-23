#include "gtest/gtest.h"
#include "RenderPassBuilder.h"
#include "StencilPopRenderModel.h"
#include "StencilPushRenderModel.h"
#include "ViewportRenderModel.h"
#include "MaterialRenderModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "WorldNode.h"
#include "Viewport.h"

using namespace PJ;
using namespace std;

namespace RenderPassBuilderTests {
    /// Test renderer with caller-controlled pre and post lists. Each test composes the
    /// shape it wants without dragging in the full MaterialRendererCore.
    class TestRenderer : public Renderer {
    public:
        RenderModelList preModels;
        RenderModelList postModels;

        RenderModelList RenderModels() override {
            return preModels;
        }

        RenderModelList PostRenderModels() override {
            return postModels;
        }

        Vector3 WorldSize() const override { return {}; }
        void SetWorldSize(Vector3) override {}
    };

    /// Walks the DAG in DFS pre-order, returning the cores. Models without a core (e.g.
    /// the synthetic root holder) are skipped.
    VectorList<SP<SomeRenderModel>> CollectDfs(RenderPassModel const& pass) {
        VectorList<SP<SomeRenderModel>> result;
        std::function<void(RenderPassModel::NodeSharedPtr const&)> walk =
            [&](auto const& node) {
                if (!node) return;
                if (node->core) result.push_back(node->core);
                for (auto& edge : node->Edges()) {
                    if (!edge || !edge->toNode) continue;
                    auto child = edge->toNode->Value();
                    if (!child) continue;
                    walk(SCAST<RenderPassModel::Node>(child));
                }
            };
        walk(pass.rootNode);
        return result;
    }
}

using namespace RenderPassBuilderTests;

TEST(RenderPassBuilder, EmptyEntries_BuildsEmptyDag)
{
    RenderPassBuilder builder;
    auto pass = builder.Build({});

    ASSERT_TRUE(pass);
    ASSERT_TRUE(pass->rootNode);
    EXPECT_EQ(0, CollectDfs(*pass).size());
}

TEST(RenderPassBuilder, FlatEntries_AttachAsChildrenOfRoot)
{
    auto r1 = MAKE<TestRenderer>();
    auto m1 = MAKE<MaterialRenderModel>();
    r1->preModels.push_back(m1);

    auto r2 = MAKE<TestRenderer>();
    auto m2 = MAKE<MaterialRenderModel>();
    r2->preModels.push_back(m2);

    RenderPassBuilder builder;
    auto pass = builder.Build({
        { .worldNode = nullptr, .preModels = r1->preModels, .postModels = r1->postModels },
        { .worldNode = nullptr, .preModels = r2->preModels, .postModels = r2->postModels },
    });

    auto dfs = CollectDfs(*pass);
    ASSERT_EQ(2, dfs.size());
    EXPECT_EQ(m1, dfs[0]);
    EXPECT_EQ(m2, dfs[1]);
}

TEST(RenderPassBuilder, PostModels_RenderAfterDescendants)
{
    // parent emits [pre, post]; child emits [child].
    // Expected DFS order: pre, child, post.
    auto parentNode = MAKE<WorldNode>();
    auto childNode = MAKE<WorldNode>();
    parentNode->Add(childNode);

    auto parentR = MAKE<TestRenderer>();
    auto pre = MAKE<MaterialRenderModel>();
    auto post = MAKE<MaterialRenderModel>();
    parentR->preModels.push_back(pre);
    parentR->postModels.push_back(post);

    auto childR = MAKE<TestRenderer>();
    auto childModel = MAKE<MaterialRenderModel>();
    childR->preModels.push_back(childModel);

    RenderPassBuilder builder;
    auto pass = builder.Build({
        { .worldNode = parentNode.get(),
          .preModels = parentR->preModels,
          .postModels = parentR->postModels },
        { .worldNode = childNode.get(),
          .preModels = childR->preModels,
          .postModels = childR->postModels },
    });

    auto dfs = CollectDfs(*pass);
    ASSERT_EQ(3, dfs.size());
    EXPECT_EQ(pre, dfs[0]);
    EXPECT_EQ(childModel, dfs[1]);
    EXPECT_EQ(post, dfs[2]);
}

TEST(RenderPassBuilder, ViewportWrapper_BecomesEffectiveRoot)
{
    auto wrapper = MAKE<ViewportRenderModel>();

    auto r = MAKE<TestRenderer>();
    auto m = MAKE<MaterialRenderModel>();
    r->preModels.push_back(m);

    RenderPassBuilder builder;
    RenderPassBuilder::Config config;
    config.wrapperModel = wrapper;

    auto pass = builder.Build(
        { { .worldNode = nullptr,
            .preModels = r->preModels,
            .postModels = r->postModels } },
        config
    );

    auto dfs = CollectDfs(*pass);
    ASSERT_EQ(2, dfs.size());
    EXPECT_EQ(wrapper, dfs[0]);
    EXPECT_EQ(m, dfs[1]);
}

TEST(RenderPassBuilder, BlendInheritance_OpaqueChildInheritsBlendFromParent)
{
    auto blendedMaterial = MAKE<RenderMaterial>();
    blendedMaterial->SetFeatureStates({ { RenderFeature::Blend, RenderFeatureState::Enable } });

    auto opaqueMaterial = MAKE<RenderMaterial>();
    opaqueMaterial->SetFeatureStates({ { RenderFeature::Blend, RenderFeatureState::Disable } });

    auto parentNode = MAKE<WorldNode>();
    auto childNode = MAKE<WorldNode>();
    parentNode->Add(childNode);

    auto parentR = MAKE<TestRenderer>();
    auto parentMat = MAKE<MaterialRenderModel>();
    parentMat->SetMaterial(blendedMaterial.get());
    parentR->preModels.push_back(parentMat);

    auto childR = MAKE<TestRenderer>();
    auto childMat = MAKE<MaterialRenderModel>();
    childMat->SetMaterial(opaqueMaterial.get());
    childR->preModels.push_back(childMat);

    RenderPassBuilder builder;
    auto pass = builder.Build({
        { .worldNode = parentNode.get(),
          .preModels = parentR->preModels,
          .postModels = parentR->postModels },
        { .worldNode = childNode.get(),
          .preModels = childR->preModels,
          .postModels = childR->postModels },
    });

    EXPECT_FALSE(childMat->IsFeatureEnabled(RenderFeature::Blend));
    RenderPassBuilder::ApplyBlendInheritance(*pass);
    EXPECT_TRUE(childMat->IsFeatureEnabled(RenderFeature::Blend));
}

TEST(RenderPassBuilder, BlendInheritance_StopsAtViewportBoundary)
{
    auto blendedMaterial = MAKE<RenderMaterial>();
    blendedMaterial->SetFeatureStates({ { RenderFeature::Blend, RenderFeatureState::Enable } });

    auto opaqueMaterial = MAKE<RenderMaterial>();
    opaqueMaterial->SetFeatureStates({ { RenderFeature::Blend, RenderFeatureState::Disable } });

    auto parentNode = MAKE<WorldNode>();
    auto childNode = MAKE<WorldNode>();
    parentNode->Add(childNode);

    auto parentR = MAKE<TestRenderer>();
    auto parentMat = MAKE<MaterialRenderModel>();
    parentMat->SetMaterial(blendedMaterial.get());
    parentR->preModels.push_back(parentMat);

    auto childR = MAKE<TestRenderer>();
    auto childMat = MAKE<MaterialRenderModel>();
    childMat->SetMaterial(opaqueMaterial.get());
    // Inject a viewport boundary as the renderer's pre[0]; the actual material follows.
    auto viewport = MAKE<ViewportRenderModel>();
    childR->preModels.push_back(viewport);
    childR->preModels.push_back(childMat);

    RenderPassBuilder builder;
    auto pass = builder.Build({
        { .worldNode = parentNode.get(),
          .preModels = parentR->preModels,
          .postModels = parentR->postModels },
        { .worldNode = childNode.get(),
          .preModels = childR->preModels,
          .postModels = childR->postModels },
    });

    RenderPassBuilder::ApplyBlendInheritance(*pass);
    // The child material sits below a viewport boundary, so it must NOT inherit Blend.
    EXPECT_FALSE(childMat->IsFeatureEnabled(RenderFeature::Blend));
}

TEST(RenderPassBuilder, ViewportCamera_WrapperMarksDagBoundary)
{
    // The wrapper-as-effective-root is how `RenderWorldSystem` flags a viewport camera's
    // DAG. Verify that a ViewportRenderModel can carry a Camera reference for the engine.
    auto wrapper = MAKE<ViewportRenderModel>();
    Viewport fakeCamera;
    wrapper->viewportCamera = &fakeCamera;

    auto r = MAKE<TestRenderer>();
    r->preModels.push_back(MAKE<MaterialRenderModel>());

    RenderPassBuilder builder;
    RenderPassBuilder::Config config;
    config.wrapperModel = wrapper;
    auto pass = builder.Build(
        { { .worldNode = nullptr,
            .preModels = r->preModels,
            .postModels = r->postModels } },
        config
    );

    auto dfs = CollectDfs(*pass);
    ASSERT_GE(dfs.size(), 1u);
    auto* vp = dynamic_cast<ViewportRenderModel*>(dfs[0].get());
    ASSERT_TRUE(vp);
    EXPECT_EQ(&fakeCamera, vp->viewportCamera);
}
