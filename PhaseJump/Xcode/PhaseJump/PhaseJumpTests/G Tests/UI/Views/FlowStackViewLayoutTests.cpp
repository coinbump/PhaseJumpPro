#include "gtest/gtest.h"
#include "FlowStackViewLayout.h"
#include "View2D.h"
#include "World.h"
#include "WorldNode.h"
#include "SpacerView.h"
#include "ViewBuilder.h"
#include "QuickBuilder.h"

using namespace std;
using namespace PJ;

namespace HStackViewLayoutTests {
}

using namespace HStackViewLayoutTests;

// FUTURE: over time, update unit tests to use ViewBuilder
TEST(FlowStackViewLayout, AllFixedSizeNoSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    View2D* stackView{};
    VectorList<View2D*> views;
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.HStack({.spacing = 0, .buildViewFunc = [&](auto& vb) {
            stackView = vb.ActiveView();
            
            vb
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetFixedSize(10, 10);
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetFixedSize(20, 20);
                }});
        }});
    });
    
    ASSERT_NE(nullptr, stackView);
    ASSERT_EQ(2, views.size());

    EXPECT_EQ(Vector2(30, 20), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 5), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(10, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
}

TEST(FlowStackViewLayout, AllFixedSizeWithSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    View2D* stackView{};
    VectorList<View2D*> views;
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.HStack({.spacing = 10, .buildViewFunc = [&](auto& vb) {
            stackView = vb.ActiveView();
            
            vb
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetFixedSize(10, 10);
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetFixedSize(20, 20);
                }});
        }});
    });
    
    ASSERT_NE(nullptr, stackView);
    ASSERT_EQ(2, views.size());
    
    EXPECT_EQ(Vector2(40, 20), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 5), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(20, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
}

TEST(FlowStackViewLayout, MixedFixedSizeNoSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    View2D* stackView{};
    VectorList<View2D*> views;
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.HStack({.spacing = 0, .buildViewFunc = [&](auto& vb) {
            stackView = vb.ActiveView();
            
            vb
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetFixedSize(20, 20);
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                }});
        }});
    });
    
    ASSERT_NE(nullptr, stackView);
    ASSERT_EQ(3, views.size());

    EXPECT_EQ(Vector2(100, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(40, 100), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 40), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(60, 0), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(40, 100), views[2]->Frame().size);
}

TEST(FlowStackViewLayout, MixedFixedSizeWithSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    View2D* stackView{};
    VectorList<View2D*> views;
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.HStack({.spacing = 10, .buildViewFunc = [&](auto& vb) {
            stackView = vb.ActiveView();
            
            vb
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetFixedSize(20, 20);
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                }});
        }});
    });
    
    ASSERT_NE(nullptr, stackView);
    ASSERT_EQ(3, views.size());

    EXPECT_EQ(Vector2(100, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(30, 100), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 40), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(70, 0), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(30, 100), views[2]->Frame().size);
}

TEST(FlowStackViewLayout, MixedFixedSizeWithSpacingAndMaxWidthConstraint) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    View2D* stackView{};
    VectorList<View2D*> views;
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.HStack({.spacing = 10, .buildViewFunc = [&](auto& vb) {
            stackView = vb.ActiveView();
            
            vb
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetMaxSize(10, {});
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                    view.SetFixedSize(20, 20);
                }})
                .View({.modifyViewFunc = [&](auto& view) {
                    views.push_back(&view);
                }});
        }});
    });
    
    ASSERT_NE(nullptr, stackView);
    ASSERT_EQ(3, views.size());

    EXPECT_EQ(Vector2(100, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 100), views[0]->Frame().size);
    EXPECT_EQ(Vector2(20, 40), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(50, 0), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(50, 100), views[2]->Frame().size);
}

std::tuple<SP<WorldNode>, SP<View2D>> Make(SP<WorldNode> parentNode) {
    auto node = MAKE<WorldNode>();
    auto view = MAKE<View2D>();
    
    if (parentNode) {
        parentNode->Add(node);
    }
    node->Add(view);
    
    return {node, view};
}

TEST(VStackAndHStackViewLayout, MixedFixedSizeNoSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto vStackT = Make(parentNode);
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y, 0);
    std::get<1>(vStackT)->SetLayout(layout);
    
    auto hStackT = Make(std::get<0>(vStackT));
    auto bottomT = Make(std::get<0>(vStackT));
    std::get<1>(bottomT)->SetFixedSize(FloatMath::maxValue, 20);
    
    layout = NEW<FlowStackViewLayout>(Axis2D::X, 0);
    std::get<1>(hStackT)->SetLayout(layout);

    VectorList<View2D*> views;
    
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        std::get<0>(hStackT)->Add(node);
        node->Add(view);
        
        if (i == 1) {
            view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        } else {
            view->SetFixedSize(FloatMath::maxValue, FloatMath::maxValue);
        }
    }
    
    parentView->SetFrameSize({100, 100});
    std::get<1>(vStackT)->LayoutIfNeeded();
    
    EXPECT_EQ(Vector2(100, 100), std::get<1>(vStackT)->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(80, 80), views[0]->Frame().size);
    EXPECT_EQ(Vector2(80, 30), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(0, 80), std::get<1>(bottomT)->Frame().origin);
    EXPECT_EQ(Vector2(100, 20), std::get<1>(bottomT)->Frame().size);
}

TEST(FlowStackViewLayout, SpacerDoesntExpandOrthogonal) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 3; i++) {
        bool isFixedSize = i == 1;
        
        auto node = MAKE<WorldNode>();
        SP<View2D> view = isFixedSize ? MAKE<View2D>() : MAKE<SpacerView>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        if (isFixedSize) {
            view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        }
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::X);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(100, 20), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(40, 20), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(60, 0), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(40, 20), views[2]->Frame().size);
}

TEST(FlowStackViewLayout, PositionConstraints) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        
        switch (i) {
            case 0:
                view->SetOffset(3, 3);
                break;
            case 1:
                view->SetPosition(12, 14);
                break;
            default:
                break;
        }
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::X);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(30, 20), stackView->Frame().size);
    EXPECT_EQ(Vector2(0 + 3, 5 + 3), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(12, 14), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
}

TEST(FlowStackViewLayout, IdealSize) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        view->SetIsIdealSize(true, true);
        view->SetIdealSizeFunc([=](auto& view, auto proposal) { return ViewSizeProposal((float)(i + 1)*10, (float)(i + 1)*10); });
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::X);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(30, 20), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 5), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(10, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
}

TEST(FlowStackViewLayout, DeriveOrthogonalSize) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        view->SetIsIdealSize(true, true);
        view->SetIdealSizeFunc([=](auto& view, auto proposal) {
            auto height = proposal.height ? *proposal.width + (i + 1) * 12 : 0;
            return ViewSizeProposal(50 - (i + 1) * 10, height);
        });
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::X);
    static_cast<FlowStackViewLayout*>(layout.get())->SetAlignFunc(AlignFuncs::left);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(70, 54), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(40, 52), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(30, 54), views[1]->Frame().size);
}

namespace VStackViewLayoutTests {
}

using namespace VStackViewLayoutTests;

TEST(VStackViewLayout, AllFixedSizeNoSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(20, 30), stackView->Frame().size);
    EXPECT_EQ(Vector2(5, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(0, 10), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
}

TEST(VStackViewLayout, AllFixedSizeWithSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 2; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y, 10);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(20, 40), stackView->Frame().size);
    EXPECT_EQ(Vector2(5, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(10, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(0, 20), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
}

TEST(VStackViewLayout, MixedFixedSizeNoSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 3; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        if (i == 1) {
            view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        } else {
            view->SetFixedSize(FloatMath::maxValue, FloatMath::maxValue);
        }
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(100, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 40), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 40), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(0, 60), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(100, 40), views[2]->Frame().size);
}

TEST(VStackViewLayout, MixedFixedSizeWithSpacing) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 3; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        if (i == 1) {
            view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        } else {
            view->SetFixedSize(FloatMath::maxValue, FloatMath::maxValue);
        }
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y, 10);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(100, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 30), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 40), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(0, 70), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(100, 30), views[2]->Frame().size);
}

TEST(VStackViewLayout, MixedFixedSizeWithSpacingAndMaxHeightConstraint) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 3; i++) {
        auto node = MAKE<WorldNode>();
        auto view = MAKE<View2D>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        if (i == 1) {
            view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        } else {
            view->SetFixedSize(FloatMath::maxValue, FloatMath::maxValue);
        }
        
        if (i == 0) {
            view->SetMaxSize({}, 10);
        }
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y, 10);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(100, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(100, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(40, 20), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(0, 50), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(100, 50), views[2]->Frame().size);
}

TEST(VStackViewLayout, SpacerDoesntExpandOrthogonal) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    auto parentView = MAKE<View2D>();
    
    world.Add(parentNode);
    parentNode->Add(parentView);

    auto stackNode = MAKE<WorldNode>();
    auto stackView = MAKE<View2D>();
    
    parentNode->Add(stackNode);
    stackNode->Add(stackView);
    
    VectorList<View2D*> views;
    
    for (int i = 0; i < 3; i++) {
        bool isFixedSize = i == 1;
        
        auto node = MAKE<WorldNode>();
        SP<View2D> view = isFixedSize ? MAKE<View2D>() : MAKE<SpacerView>();
        views.push_back(view.get());
        
        stackNode->Add(node);
        node->Add(view);
        
        if (isFixedSize) {
            view->SetFixedSize((float)(i + 1)*10, (float)(i + 1)*10);
        }
    }
    
    parentView->SetFrameSize({100, 100});
    
    UP<SomeViewLayout> layout = NEW<FlowStackViewLayout>(Axis2D::Y);
    stackView->SetLayout(layout);
    stackView->LayoutIfNeeded();

    EXPECT_EQ(Vector2(20, 100), stackView->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(20, 40), views[0]->Frame().size);
    EXPECT_EQ(Vector2(0, 40), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(20, 20), views[1]->Frame().size);
    EXPECT_EQ(Vector2(0, 60), views[2]->Frame().origin);
    EXPECT_EQ(Vector2(20, 40), views[2]->Frame().size);
}

// With an HStack inside another HStack and the child of the second HStack has an unbounded axis length
// Then the parent HStack of the unbounded child should have an unbounded child length also
TEST(FlowStackViewLayout, AllChildrenUnboundedLength) {
    World world;
    auto parentNode = MAKE<WorldNode>();
    world.Add(parentNode);
    
    View2D* stackView{};
    VectorList<View2D*> views;
    
    ViewBuilder vb(*parentNode);
    vb.RootView({100, 100}, [&](auto& vb) {
        vb.HStack({ .buildViewFunc = [&](auto& vb) {
            vb.HStack(
                      { .buildViewFunc =
                          [&](auto& vb) {
                              vb.View({})
                              .QB().template ModifyLatest<View2D>([&](auto& view) {
                                  views.push_back(&view);
                                  view.SetFixedSize({}, 10);
                              });
                          } }
                      ).HStack({ .buildViewFunc = [&](auto& vb) {
                          vb.View({})
                          .QB().template ModifyLatest<View2D>([&](auto& view) {
                              views.push_back(&view);
                              view.SetFixedSize(10, {});
                          });
                      }});
        }});
    });
    
    ASSERT_EQ(2, views.size());

    auto parentHStack = views[0]->ParentView();
    ASSERT_NE(nullptr, parentHStack);
    
    EXPECT_EQ(Vector2(0, 45), parentHStack->Frame().origin);
    EXPECT_EQ(Vector2(0, 0), views[0]->Frame().origin);
    EXPECT_EQ(Vector2(90, 10), views[0]->Frame().size);
    EXPECT_EQ(Vector2(0, 0), views[1]->Frame().origin);
    EXPECT_EQ(Vector2(10, 100), views[1]->Frame().size);
}
