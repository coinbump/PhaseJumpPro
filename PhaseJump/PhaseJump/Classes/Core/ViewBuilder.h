#pragma once

#include "Binding.h"
#include "ButtonControl.h"
#include "Color.h"
#include "Font.h"
#include "LayoutInsets.h"
#include "PageView.h"
#include "SomeAligner.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/4/24
 */
namespace PJ {
    class QuickBuild;
    class WorldNode;
    class SomeCollider2D;
    class ImRenderer;
    class DesignSystem;

    /// Used to build views with QuickBuild
    class ViewBuilder {
    protected:
        /// Builder for target node
        UP<QuickBuild> qb;

    public:
        using This = ViewBuilder;

        using BuildViewFunc = std::function<void(This&)>;
        using AddViewFunc = std::function<void(QuickBuild&)>;
        using MakeColliderFunc = std::function<SP<SomeCollider2D>()>;

        template <class Config>
        using BuildConfigFunc = std::function<Config(View2D&)>;

        /// Generic view config
        struct ViewConfig {
            using ModifyViewFunc = std::function<void(View2D&)>;

            String name = "View";
            ModifyViewFunc modifyViewFunc;
        };

        /// Pad view config
        struct PadConfig {
            String name = "Pad";
            LayoutInsets insets;
            BuildViewFunc buildViewFunc;
        };

        /// HStack view config
        struct HStackConfig {
            String name = "HStack";
            float spacing{};
            AlignFunc alignFunc = AlignFuncs::center;
            BuildViewFunc buildViewFunc;
        };

        /// VStack view config
        struct VStackConfig {
            String name = "VStack";
            float spacing{};
            AlignFunc alignFunc = AlignFuncs::center;
            BuildViewFunc buildViewFunc;
        };

        /// ZStack view config
        struct ZStackConfig {
            String name = "ZStack";
            AlignFunc xAlignFunc = AlignFuncs::center;
            AlignFunc yAlignFunc = AlignFuncs::center;
            BuildViewFunc buildViewFunc;
        };

        /// MatchZStack view config
        struct MatchZStackConfig {
            String name = "MatchZStack";

            BuildViewFunc buildBackgroundFunc;
            BuildViewFunc buildViewFunc;
            BuildViewFunc buildOverlayFunc;
        };

        /// Color view config
        struct ColorConfig {
            String name = "Color";
            Color color;
        };

        /// Text view config
        struct TextConfig {
            String name = "Text";
            FontSpec fontSpec = { .size = 12 };
            String text;
            Color color = Color::white;
        };

        /// Slider control config
        struct SliderConfig {
            String name = "Slider";
            Axis2D axis = Axis2D::X;
            Binding<float> valueBinding;

            /// Orthogonal size of the slider track perpendicular to the primary axis
            float trackOrthogonal = 10;
        };

        /// Page view config
        struct PagesConfig {
            String name = "Pages";
            String selectedPage;

            SP<PageViewStore> store = MAKE<PageViewStore>();

            VectorList<PageView::PageConfig> pages;
        };

        /// Immediate render view config
        struct ImmediateConfig {
            using RenderFunc = std::function<void(View2D&, ImRenderer&)>;
            using ModifyRendererFunc = std::function<void(ImRenderer&)>;

            String name = "Immediate";
            RenderFunc renderFunc;
            ModifyRendererFunc modifyRendererFunc;
        };

        /// Button config
        struct ButtonConfig {
            String name = "Button";
            UIControl2D::OnControlChangeFunc onControlChangeFunc;
            MakeColliderFunc makeColliderFunc;
            BuildViewFunc buildFrameFunc;
            BuildViewFunc buildLabelFunc;
            ButtonControl::OnPressFunc onPressFunc;
        };

        /// Toggle button config
        struct ToggleButtonConfig {
            String name = "Toggle Button";
            UIControl2D::OnControlChangeFunc onControlChangeFunc;
            MakeColliderFunc makeColliderFunc;
            Binding<bool> isOnBinding;
            BuildViewFunc buildFrameFunc;
            BuildViewFunc buildLabelFunc;
        };

        ViewBuilder(WorldNode& node);

        QuickBuild& QB() {
            return *qb;
        }

        World* GetWorld() const;
        DesignSystem* GetDesignSystem() const;

        /// @return Returns the active view where new views will be created
        View2D* ActiveView() const;

        /// Starts with a root view
        This& RootView(Vector2 size, ViewBuilder::BuildViewFunc buildFunc);

        /// Adds a generic view
        This& View(ViewConfig config);

        /// Adds a dynamic pad view
        This& Pad(BuildConfigFunc<PadConfig> buildConfigFunc);

        /// Adds a pad view
        This& Pad(PadConfig config) {
            return Pad([=](auto& view) { return config; });
        }

        /// Adds a dynamic HStack view
        This& HStack(BuildConfigFunc<HStackConfig> config);

        /// Adds a HStack view
        This& HStack(HStackConfig config) {
            return HStack([=](auto& view) { return config; });
        }

        /// Adds a dynamic VStack view
        This& VStack(BuildConfigFunc<VStackConfig> config);

        /// Adds a VStack view
        This& VStack(VStackConfig config) {
            return VStack([=](auto& view) { return config; });
        }

        /// Adds a dynamic ZStack view
        This& ZStack(BuildConfigFunc<ZStackConfig> config);

        /// Adds a ZStack view
        This& ZStack(ZStackConfig config) {
            return ZStack([=](auto& view) { return config; });
        }

        /// Adds a dynamic ZStack view with an optional background and overlay that match the size
        /// of the content view
        This& MatchZStack(BuildConfigFunc<MatchZStackConfig> buildConfigFunc);

        /// Adds a ZStack view with an optional background and overlay that match the size of the
        /// content view
        This& MatchZStack(MatchZStackConfig config) {
            return MatchZStack([=](auto& view) { return config; });
        }

        /// Adds a dynamic Color view
        This& Color(BuildConfigFunc<ColorConfig> buildConfigFunc);

        /// Adds a Color view
        This& Color(ColorConfig config) {
            return Color([=](auto& view) { return config; });
        }

        /// Adds a Color view
        This& Color(PJ::Color color) {
            return Color(ColorConfig{ .color = color });
        }

        /// Adds a dynamic button view
        This& Button(BuildConfigFunc<ButtonConfig> buildConfigFunc);

        /// Adds a button view
        This& Button(ButtonConfig config) {
            return Button([=](auto& view) { return config; });
        }

        /// Adds a dynamic intermediate render view
        This& Immediate(BuildConfigFunc<ImmediateConfig> buildConfigFunc);

        /// Adds an intermediate render view
        This& Immediate(ImmediateConfig config) {
            return Immediate([=](auto& view) { return config; });
        }

        /// Adds a dynamic Page view
        This& Pages(BuildConfigFunc<PagesConfig> buildConfigFunc);

        /// Adds a Page view
        This& Pages(PagesConfig config) {
            return Pages([=](auto& view) { return config; });
        };

        /// Adds a dynamc Text view
        This& Text(BuildConfigFunc<TextConfig> buildConfigFunc);

        /// Adds a Text view
        This& Text(TextConfig config) {
            return Text([=](auto& view) { return config; });
        }

        /// Adds a dynamic toggle button view
        This& ToggleButton(BuildConfigFunc<ToggleButtonConfig> buildConfigFunc);

        /// Adds a toggle button view
        This& ToggleButton(ToggleButtonConfig config) {
            return ToggleButton([=](auto& view) { return config; });
        }

        /// Adds a slider
        This& Slider(SliderConfig config);

        /// Adds a Spacer view
        This& Spacer();

        // MARK: View modifiers

        This& Background(BuildViewFunc buildViewFunc);

        // MARK: Constraints

        /// Adds offset constraint
        This& Offset(float x = 0, float y = 0);

        /// Sets fixed size constraints for the most recent view
        This& FixedSize(std::optional<float> width, std::optional<float> height);

        /// Sets is ideal size for the most recent view
        This& SetIsIdealSize(bool isX = true, bool isY = true);

    protected:
        This& AddMatchZStack(View2D& view, MatchZStackConfig config);
    };
} // namespace PJ
