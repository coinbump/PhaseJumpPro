#include "gtest/gtest.h"

#include "ImRenderer.h"
#include "MaterialRenderModel.h"
#include "SomeShape.h"
#include "StencilPopRenderModel.h"
#include "StencilPushRenderModel.h"

using namespace std;
using namespace PJ;

namespace ImRendererTests {
    RenderModelList BuildModels(ImRenderer& renderer) {
        return renderer.RenderModels();
    }

    int CountPushes(RenderModelList const& models) {
        int count = 0;
        for (auto& sp : models) {
            if (dynamic_cast<StencilPushRenderModel*>(sp.get())) {
                ++count;
            }
        }
        return count;
    }

    int CountPops(RenderModelList const& models) {
        int count = 0;
        for (auto& sp : models) {
            if (dynamic_cast<StencilPopRenderModel*>(sp.get())) {
                ++count;
            }
        }
        return count;
    }

    int CountDraws(RenderModelList const& models) {
        int count = 0;
        for (auto& sp : models) {
            if (dynamic_cast<MaterialRenderModel*>(sp.get())) {
                ++count;
            }
        }
        return count;
    }
} // namespace ImRendererTests

using namespace ImRendererTests;

TEST(ImRenderer, FillRectProducesDrawModel) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });

    auto models = BuildModels(renderer);
    EXPECT_EQ(1, CountDraws(models));
    EXPECT_EQ(0, CountPushes(models));
    EXPECT_EQ(0, CountPops(models));
}

TEST(ImRenderer, PushClipProducesStencilPush) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPush({ { 10, 10 }, { 50, 50 } }, NEW<EllipseShape>(Vector2{ 50, 50 }));
    renderer.ClipPop();

    auto models = BuildModels(renderer);
    EXPECT_EQ(1, CountPushes(models));
    EXPECT_EQ(1, CountPops(models));
}

TEST(ImRenderer, PushClipWithDrawAndPop) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPush({ { 10, 10 }, { 50, 50 } }, NEW<EllipseShape>(Vector2{ 50, 50 }));
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });
    renderer.ClipPop();

    auto models = BuildModels(renderer);
    EXPECT_EQ(1, CountPushes(models));
    EXPECT_EQ(1, CountDraws(models));
    EXPECT_EQ(1, CountPops(models));
}

TEST(ImRenderer, UnmatchedPushIsAutoPopped) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPush({ { 10, 10 }, { 50, 50 } }, NEW<EllipseShape>(Vector2{ 50, 50 }));
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });

    auto models = BuildModels(renderer);
    EXPECT_EQ(1, CountPushes(models));
    EXPECT_EQ(1, CountPops(models));
}

TEST(ImRenderer, MultipleUnmatchedPushesAreBalanced) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPush({ { 10, 10 }, { 50, 50 } }, NEW<RectShape>(Vector2{ 50, 50 }));
    renderer.ClipPush({ { 20, 20 }, { 30, 30 } }, NEW<EllipseShape>(Vector2{ 30, 30 }));
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });

    auto models = BuildModels(renderer);
    EXPECT_EQ(2, CountPushes(models));
    EXPECT_EQ(2, CountPops(models));
}

TEST(ImRenderer, PopWithoutPushIsIgnored) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPop();
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });

    auto models = BuildModels(renderer);
    EXPECT_EQ(0, CountPushes(models));
    EXPECT_EQ(0, CountPops(models));
    EXPECT_EQ(1, CountDraws(models));
}

TEST(ImRenderer, PushClipOrderIsPushBeforeDraw) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPush({ { 10, 10 }, { 50, 50 } }, NEW<EllipseShape>(Vector2{ 50, 50 }));
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });
    renderer.ClipPop();

    auto models = BuildModels(renderer);
    ASSERT_EQ(3u, models.size());
    EXPECT_TRUE(dynamic_cast<StencilPushRenderModel*>(models[0].get()) != nullptr);
    EXPECT_TRUE(dynamic_cast<MaterialRenderModel*>(models[1].get()) != nullptr);
    EXPECT_TRUE(dynamic_cast<StencilPopRenderModel*>(models[2].get()) != nullptr);
}

TEST(ImRenderer, ClipPushRectConvenienceUsesRectShape) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPush({ { 10, 10 }, { 50, 50 } });
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });
    renderer.ClipPop();

    auto models = BuildModels(renderer);
    EXPECT_EQ(1, CountPushes(models));
    EXPECT_EQ(1, CountDraws(models));
    EXPECT_EQ(1, CountPops(models));
}

TEST(ImRenderer, PushNullShapeIsIgnored) {
    ImRenderer renderer(ImRenderer::Config{ .worldSize = { 100, 100 } });
    renderer.ClipPush({ { 10, 10 }, { 50, 50 } }, nullptr);
    renderer.FillRect({ { 0, 0 }, { 10, 10 } });

    auto models = BuildModels(renderer);
    EXPECT_EQ(0, CountPushes(models));
    EXPECT_EQ(0, CountPops(models));
    EXPECT_EQ(1, CountDraws(models));
}
