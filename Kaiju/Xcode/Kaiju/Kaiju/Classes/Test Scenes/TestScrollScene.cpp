#include "TestScrollScene.h"

using namespace std;
using namespace PJ;

namespace {
    constexpr float sceneWidth = 900.0f;
    constexpr float sceneHeight = 600.0f;
    constexpr float sliderWidth = 33.0f;
    constexpr float scrollWidth = sceneWidth - sliderWidth;
    constexpr float textWidth = scrollWidth;

    auto const loremIpsumParagraphs =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt "
        "ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
        "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
        "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\n"

        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit "
        "anim id est laborum. Curabitur pretium tincidunt lacus. Nulla gravida orci a odio. Nullam "
        "varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis "
        "sollicitudin mauris.\n\n"

        "Integer in mauris eu nibh euismod gravida. Duis ac tellus et risus vulputate vehicula. "
        "Donec lobortis risus a elit. Etiam tempor. Ut ullamcorper, ligula eu tempor congue, eros "
        "est euismod turpis, id tincidunt sapien risus a quam. Maecenas fermentum consequat mi. "
        "Donec fermentum. Pellentesque malesuada nulla a mi.\n\n"

        "Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque. Aliquam faucibus, elit "
        "ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat. "
        "Cras mollis scelerisque nunc. Nullam arcu. Aliquam consequat. Curabitur augue lorem, "
        "dapibus quis, laoreet et, pretium ac, nisi.";
} // namespace

void TestScrollScene::LoadInto(WorldNode& root) {
    QB(root).Named("Test Scroll").OrthoStandard().SizeWithWindow();

    auto world = root.World();
    GUARD(world)

    auto& rootViewNode = root.And(WorldNode::Config{ .name = "Scroll Root" });
    auto& rootView = rootViewNode.AddComponent<View2D>();
    rootView.SetFrameSize({ sceneWidth, sceneHeight });

    auto& contentNode = rootViewNode.And(WorldNode::Config{ .name = "Content" });
    auto& contentView = contentNode.AddComponent<View2D>();
    UP<SomeViewLayout> contentLayout = NEW<FlowStackViewLayout>(Axis2D::X, 0);
    contentView.SetLayout(contentLayout);

    // Scroll view with a single child containing the text.
    auto& scrollNode = contentNode.And(WorldNode::Config{ .name = "Scroll" });
    auto& scroll = scrollNode.AddComponent<ScrollView>();
    scroll.SetFixedSize(scrollWidth, sceneHeight);
    scroll.Clip();
    scrollView = SCAST<ScrollView>(scroll.shared_from_this());

    auto& textNode = scrollNode.And(WorldNode::Config{ .name = "Text" });
    auto& textView = textNode.AddComponent<View2D>();

    auto font = FindFontWithSize(world->resources, 18);
    auto& textRenderer = textNode.AddComponent<TextRenderer>(TextRenderer::Config{
        .font = font,
        .text = AttributedString(loremIpsumParagraphs),
        .worldSize = { textWidth, 0 } });
    textRenderer.SetTextColor(Color::black)
        .SetLineAlignFunc(AlignFuncs::left)
        .SetTextAlignFunc(AlignFuncs::center)
        .SetLineClip(LineClip::None);

    float const textHeight = textRenderer.CalculateSize({ textWidth, 0 }).y;
    textView.SetFixedSize(textWidth, textHeight);
    textRenderer.SetWorldSize({ textWidth, textHeight, 0 });

    float const maxScroll = std::max(0.0f, textHeight - sceneHeight);

    Binding<float> scrollBinding({ .getFunc =
                                       [this, maxScroll]() {
                                           auto _scrollView = LOCK(scrollView);
                                           GUARDR(_scrollView && maxScroll > 0, 0.0f)
                                           return _scrollView->Position().y / maxScroll;
                                       },
                                   .setFunc =
                                       [this, maxScroll](auto& value) {
                                           auto _scrollView = LOCK(scrollView);
                                           GUARD(_scrollView)
                                           auto pos = _scrollView->Position();
                                           pos.y = value * maxScroll;
                                           _scrollView->SetPosition(pos);
                                       } });

    QB(contentNode)
        .And(WorldNode::Config{ .name = "Slider" })
        .Slider({ .axis = Axis2D::Y,
                  .valueBinding = scrollBinding,
                  .worldSize = { sliderWidth, sceneHeight } })
        .ModifyLatest<SliderControl>([](auto& slider) {
            slider.SetFixedSize(sliderWidth, sceneHeight);
        });
}
