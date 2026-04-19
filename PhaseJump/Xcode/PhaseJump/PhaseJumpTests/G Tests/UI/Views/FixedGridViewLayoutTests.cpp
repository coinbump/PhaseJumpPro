#include "gtest/gtest.h"
#include "FixedGridViewLayout.h"
#include "View2D.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

namespace FixedGridViewLayoutTests {
}

using namespace FixedGridViewLayoutTests;

TEST(FixedGridViewLayout, ViewSize_Default) {
    FixedGridViewLayout::Config config;
    config.gridSize = { 2, 2 };
    config.cellSize = { 100, 100 };
    config.spacing = {};

    FixedGridViewLayout layout(config);

    VectorList<ViewProxy> children;
    ViewSizeProposal proposal{};

    // 2 * 100 + (2-1) * 0 = 200
    auto size = layout.ViewSize(proposal, children);
    EXPECT_EQ(Vector2(200, 200), size);
}

TEST(FixedGridViewLayout, ViewSize_WithSpacing) {
    FixedGridViewLayout::Config config;
    config.gridSize = { 3, 2 };
    config.cellSize = { 50, 80 };
    config.spacing = { 10, 20 };

    FixedGridViewLayout layout(config);

    VectorList<ViewProxy> children;
    ViewSizeProposal proposal{};

    // Width: 3 * 50 + (3-1) * 10 = 150 + 20 = 170
    // Height: 2 * 80 + (2-1) * 20 = 160 + 20 = 180
    auto size = layout.ViewSize(proposal, children);
    EXPECT_EQ(Vector2(170, 180), size);
}

TEST(FixedGridViewLayout, ViewSize_SingleCell) {
    FixedGridViewLayout::Config config;
    config.gridSize = { 1, 1 };
    config.cellSize = { 64, 64 };
    config.spacing = { 10, 10 };

    FixedGridViewLayout layout(config);

    VectorList<ViewProxy> children;
    ViewSizeProposal proposal{};

    // 1 * 64 + (1-1) * 10 = 64
    auto size = layout.ViewSize(proposal, children);
    EXPECT_EQ(Vector2(64, 64), size);
}

TEST(FixedGridViewLayout, ViewSize_ZeroGridSize) {
    FixedGridViewLayout::Config config;
    config.gridSize = { 0, 0 };
    config.cellSize = { 100, 100 };

    FixedGridViewLayout layout(config);

    VectorList<ViewProxy> children;
    ViewSizeProposal proposal{};

    auto size = layout.ViewSize(proposal, children);
    EXPECT_EQ(Vector2(0, 0), size);
}

TEST(FixedGridViewLayout, Layout_2x2_FixedChildren) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();

    world.Add(parentNode);
    parentNode->Add(parentView);

    auto gridNode = MAKE<WorldNode>();
    auto gridView = MAKE<View2D>();

    parentNode->Add(gridNode);
    gridNode->Add(gridView);

    VectorList<View2D*> views;

    for (int i = 0; i < 4; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());

        gridNode->Add(node);
        node->Add(view);

        view->SetFixedSize(100, 100);
    }

    parentView->SetFrameSize({ 400, 400 });

    FixedGridViewLayout::Config config;
    config.gridSize = { 2, 2 };
    config.cellSize = { 100, 100 };
    config.spacing = {};

    UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(config);
    gridView->SetLayout(layout);
    gridView->LayoutIfNeeded();

    // Grid view size: 2 * 100 = 200 x 200
    EXPECT_EQ(Vector2(200, 200), gridView->Frame().size);

    // Row 0: (0,0) and (100,0)
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), views[0]->Frame().size);

    EXPECT_EQ(Vector2(100, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), views[1]->Frame().size);

    // Row 1: (0,100) and (100,100)
    EXPECT_EQ(Vector2(0, 100), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), views[2]->Frame().size);

    EXPECT_EQ(Vector2(100, 100), views[3]->Frame().origin);
    EXPECT_EQ(Vector2(100, 100), views[3]->Frame().size);
}

TEST(FixedGridViewLayout, Layout_WithSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();

    world.Add(parentNode);
    parentNode->Add(parentView);

    auto gridNode = MAKE<WorldNode>();
    auto gridView = MAKE<View2D>();

    parentNode->Add(gridNode);
    gridNode->Add(gridView);

    VectorList<View2D*> views;

    for (int i = 0; i < 4; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());

        gridNode->Add(node);
        node->Add(view);

        view->SetFixedSize(50, 50);
    }

    parentView->SetFrameSize({ 400, 400 });

    FixedGridViewLayout::Config config;
    config.gridSize = { 2, 2 };
    config.cellSize = { 50, 50 };
    config.spacing = { 10, 20 };

    UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(config);
    gridView->SetLayout(layout);
    gridView->LayoutIfNeeded();

    // Grid view size: 2*50 + 1*10 = 110, 2*50 + 1*20 = 120
    EXPECT_EQ(Vector2(110, 120), gridView->Frame().size);

    // Row 0
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(60, 0), views[1]->Frame().origin);  // 50 + 10 spacing

    // Row 1
    EXPECT_EQ(Vector2(0, 70), views[2]->Frame().origin);   // 50 + 20 spacing
    EXPECT_EQ(Vector2(60, 70), views[3]->Frame().origin);
}

TEST(FixedGridViewLayout, Layout_3x2_Grid) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();

    world.Add(parentNode);
    parentNode->Add(parentView);

    auto gridNode = MAKE<WorldNode>();
    auto gridView = MAKE<View2D>();

    parentNode->Add(gridNode);
    gridNode->Add(gridView);

    VectorList<View2D*> views;

    for (int i = 0; i < 6; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());

        gridNode->Add(node);
        node->Add(view);

        view->SetFixedSize(40, 40);
    }

    parentView->SetFrameSize({ 400, 400 });

    FixedGridViewLayout::Config config;
    config.gridSize = { 3, 2 };
    config.cellSize = { 40, 40 };
    config.spacing = {};

    UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(config);
    gridView->SetLayout(layout);
    gridView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(120, 80), gridView->Frame().size);

    // Row 0: columns 0, 1, 2
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(40, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(80, 0), views[2]->Frame().origin);

    // Row 1: columns 0, 1, 2
    EXPECT_EQ(Vector2(0, 40), views[3]->Frame().origin);
    EXPECT_EQ(Vector2(40, 40), views[4]->Frame().origin);
    EXPECT_EQ(Vector2(80, 40), views[5]->Frame().origin);
}

TEST(FixedGridViewLayout, Layout_FewerChildrenThanCells) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();

    world.Add(parentNode);
    parentNode->Add(parentView);

    auto gridNode = MAKE<WorldNode>();
    auto gridView = MAKE<View2D>();

    parentNode->Add(gridNode);
    gridNode->Add(gridView);

    VectorList<View2D*> views;

    // Only 2 children in a 2x2 grid (4 cells)
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());

        gridNode->Add(node);
        node->Add(view);

        view->SetFixedSize(100, 100);
    }

    parentView->SetFrameSize({ 400, 400 });

    FixedGridViewLayout::Config config;
    config.gridSize = { 2, 2 };
    config.cellSize = { 100, 100 };
    config.spacing = {};

    UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(config);
    gridView->SetLayout(layout);
    gridView->LayoutIfNeeded();

    // Grid size is still 2x2 regardless of child count
    EXPECT_EQ(Vector2(200, 200), gridView->Frame().size);

    // Only first 2 cells filled
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 0), views[1]->Frame().origin);
}

TEST(FixedGridViewLayout, Layout_CenterAlignment) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();

    world.Add(parentNode);
    parentNode->Add(parentView);

    auto gridNode = MAKE<WorldNode>();
    auto gridView = MAKE<View2D>();

    parentNode->Add(gridNode);
    gridNode->Add(gridView);

    VectorList<View2D*> views;

    // Child smaller than cell: 40x40 in 100x100 cell
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();
    views.push_back(view.get());

    gridNode->Add(node);
    node->Add(view);

    view->SetFixedSize(40, 40);

    parentView->SetFrameSize({ 400, 400 });

    FixedGridViewLayout::Config config;
    config.gridSize = { 1, 1 };
    config.cellSize = { 100, 100 };
    config.alignment = Alignment2D::center;

    UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(config);
    gridView->SetLayout(layout);
    gridView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(100, 100), gridView->Frame().size);

    // Center aligned: (100-40)/2 = 30
    EXPECT_EQ(Vector2(30, 30), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(40, 40), views[0]->Frame().size);
}

TEST(FixedGridViewLayout, Layout_TopLeadingAlignment) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();

    world.Add(parentNode);
    parentNode->Add(parentView);

    auto gridNode = MAKE<WorldNode>();
    auto gridView = MAKE<View2D>();

    parentNode->Add(gridNode);
    gridNode->Add(gridView);

    VectorList<View2D*> views;

    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();
    views.push_back(view.get());

    gridNode->Add(node);
    node->Add(view);

    view->SetFixedSize(40, 40);

    parentView->SetFrameSize({ 400, 400 });

    FixedGridViewLayout::Config config;
    config.gridSize = { 1, 1 };
    config.cellSize = { 100, 100 };
    config.alignment = Alignment2D::topLeading;

    UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(config);
    gridView->SetLayout(layout);
    gridView->LayoutIfNeeded();

    // Top-leading: aligned to (0,0)
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(40, 40), views[0]->Frame().size);
}

TEST(FixedGridViewLayout, Layout_BottomTrailingAlignment) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();

    world.Add(parentNode);
    parentNode->Add(parentView);

    auto gridNode = MAKE<WorldNode>();
    auto gridView = MAKE<View2D>();

    parentNode->Add(gridNode);
    gridNode->Add(gridView);

    VectorList<View2D*> views;

    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();
    views.push_back(view.get());

    gridNode->Add(node);
    node->Add(view);

    view->SetFixedSize(40, 40);

    parentView->SetFrameSize({ 400, 400 });

    FixedGridViewLayout::Config config;
    config.gridSize = { 1, 1 };
    config.cellSize = { 100, 100 };
    config.alignment = Alignment2D::bottomTrailing;

    UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(config);
    gridView->SetLayout(layout);
    gridView->LayoutIfNeeded();

    // Bottom-trailing: x = 100-40 = 60, y = 100-40 = 60
    EXPECT_EQ(Vector2(60, 60), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(40, 40), views[0]->Frame().size);
}

TEST(FixedGridViewLayout, TypeName) {
    FixedGridViewLayout::Config config;
    FixedGridViewLayout layout(config);

    EXPECT_EQ("FixedGridViewLayout", layout.TypeName());
}
