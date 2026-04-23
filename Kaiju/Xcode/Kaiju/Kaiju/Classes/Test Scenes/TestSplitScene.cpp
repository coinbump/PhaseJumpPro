#include "TestSplitScene.h"

using namespace std;
using namespace PJ;

namespace {
    constexpr float rootSize = 2000.0f;
    constexpr float minRatio = 0.2f;
    constexpr float maxRatio = 0.8f;

    StandardRandom random;

    Axis2D RandomAxis() {
        return random.IntValue(0, 1) == 0 ? Axis2D::X : Axis2D::Y;
    }

    float RandomRatio() {
        return minRatio + random.Value() * (maxRatio - minRatio);
    }

    Color RandomColor() {
        return Color(random.Value(), random.Value(), random.Value(), 1.0f);
    }
} // namespace

void TestSplitScene::LoadInto(WorldNode& root) {
    QB(root).Named("Test Split").OrthoStandard().SizeWithWindow();

    auto& rootViewNode = root.And(WorldNode::Config{ .name = "Split Root" });
    auto& rootView = rootViewNode.AddComponent<View2D>();
    rootView.SetFrameSize({ rootSize, rootSize });

    auto& contentNode = rootViewNode.And(WorldNode::Config{ .name = "Content" });
    auto& contentView = contentNode.AddComponent<View2D>();
    UP<SomeViewLayout> stack = NEW<FlowStackViewLayout>(Axis2D::Y, 0);
    contentView.SetLayout(stack);

    // Leaf pane: a random-colored fill.
    ViewBuilder::BuildViewFunc colorFiller = [](ViewBuilder& vb) { vb.Color(RandomColor()); };

    // Inner pane: a nested random-axis / random-ratio split of two color leaves.
    ViewBuilder::BuildViewFunc innerSplitFiller = [colorFiller](ViewBuilder& vb) {
        vb.SplitView({ .axis = RandomAxis(),
                       .initialRatio = RandomRatio(),
                       .minRatio = minRatio,
                       .maxRatio = maxRatio,
                       .buildFirstViewFunc = colorFiller,
                       .buildSecondViewFunc = colorFiller });
    };

    // Three top-level splits stacked vertically, each subdivided into two inner splits.
    ViewBuilder contentVB(contentNode);
    for (int i = 0; i < 3; ++i) {
        contentVB.SplitView({ .axis = RandomAxis(),
                              .initialRatio = RandomRatio(),
                              .minRatio = minRatio,
                              .maxRatio = maxRatio,
                              .buildFirstViewFunc = innerSplitFiller,
                              .buildSecondViewFunc = innerSplitFiller });
    }
}
