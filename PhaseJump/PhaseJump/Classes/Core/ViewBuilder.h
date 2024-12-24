#pragma once

#include "AlignFunc.h"
#include "Alignment2D.h"
#include "AttributedString.h"
#include "Binding.h"
#include "ButtonControl.h"
#include "Color.h"
#include "DesignSystem.h"
#include "Font.h"
#include "LayoutInsets.h"
#include "PageView.h"
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

        using SurfaceConfig = DesignSystem::SurfaceConfig;
        using ButtonConfig = DesignSystem::ButtonConfig;
        using ToggleButtonConfig = DesignSystem::ToggleButtonConfig;
        using ProgressBarConfig = DesignSystem::ProgressBarConfig;
        using LabelConfig = DesignSystem::LabelConfig;
        using DialConfig = DesignSystem::DialConfig;
        using ToolTipConfig = DesignSystem::ToolTipConfig;
        using SliderConfig = DesignSystem::SliderConfig;

        struct RadioButtonGroupConfig {
            SingleSelectStore* store{};
            VectorList<String> options;
        };

        using SegmentedPickerConfig = RadioButtonGroupConfig;
        using ModifyViewFunc = std::function<void(View2D&)>;

        /// Generic view config
        struct ViewConfig {
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

        /// Fixed grid view config
        struct FixedGridConfig {
            String name = "FixedGrid";

            Vector2Int gridSize{ 2, 2 };
            Vector2 cellSize{ 100, 100 };

            /// Defines how content is aligned in each cell
            Alignment2D alignment = Alignment2D::center;

            /// Spacing between rows/columns
            Vector2 spacing;

            BuildViewFunc buildViewFunc;
        };

        /// ZStack view config
        struct ZStackConfig {
            String name = "ZStack";

            Alignment2D alignment = Alignment2D::center;
            BuildViewFunc buildViewFunc;
        };

        /// ViewAttachments view config
        struct ViewAttachmentsConfig {
            String name = "ViewAttachments";

            BuildViewFunc buildBackgroundFunc;

            BuildViewFunc buildViewFunc;

            // Used if we're moving a pre-existing view
            SP<WorldNode> viewNode;

            BuildViewFunc buildOverlayFunc;
            ModifyViewFunc modifyViewFunc;
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
            AttributedString text;
            Color color = Color::white;
        };

        /// Page view config
        struct PagesConfig {
            String name = "Pages";
            String selectedPage;

            SP<SingleSelectStore> store = MAKE<SingleSelectStore>();

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

        /// Slider view config
        struct SliderViewConfig {
            String name = "Slider";
            Axis2D axis = Axis2D::X;
            Binding<float> valueBinding;

            /// Orthogonal size of the slider track perpendicular to the primary axis
            float trackOrthogonal = 10;
        };

        /// Button view config
        struct ButtonViewConfig {
            String name = "Button";
            UIControl2D::OnControlChangeFunc onControlChangeFunc;
            MakeColliderFunc makeColliderFunc;
            BuildViewFunc buildFrameFunc;
            BuildViewFunc buildLabelFunc;
            ButtonControl::OnPressFunc onPressFunc;
            ModifyViewFunc modifyViewFunc;
        };

        /// Toggle button view config
        struct ToggleButtonViewConfig {
            String name = "Toggle Button";
            UIControl2D::OnControlChangeFunc onControlChangeFunc;
            MakeColliderFunc makeColliderFunc;
            Binding<bool> isOnBinding;
            BuildViewFunc buildFrameFunc;
            BuildViewFunc buildLabelFunc;
            ModifyViewFunc modifyViewFunc;
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
        This& HStack(BuildConfigFunc<HStackConfig> buildConfigFunc);

        /// Adds a HStack view
        This& HStack(HStackConfig config) {
            return HStack([=](auto& view) { return config; });
        }

        /// Adds a dynamic fixed grid layout view
        This& FixedGrid(BuildConfigFunc<FixedGridConfig> buildConfigFunc);

        /// Adds a fixed grid layout view
        This& FixedGrid(FixedGridConfig config) {
            return FixedGrid([=](auto& view) { return config; });
        }

        /// Adds a dynamic VStack view
        This& VStack(BuildConfigFunc<VStackConfig> buildConfigFunc);

        /// Adds a VStack view
        This& VStack(VStackConfig config) {
            return VStack([=](auto& view) { return config; });
        }

        /// Adds a dynamic ZStack view
        This& ZStack(BuildConfigFunc<ZStackConfig> buildConfigFunc);

        /// Adds a ZStack view
        This& ZStack(ZStackConfig config) {
            return ZStack([=](auto& view) { return config; });
        }

        /// Adds a dynamic ZStack view with an optional background and overlay that match the size
        /// of the content view
        This& ViewAttachments(BuildConfigFunc<ViewAttachmentsConfig> buildConfigFunc);

        /// Adds a ZStack view with an optional background and overlay that match the size of the
        /// content view
        This& ViewAttachments(ViewAttachmentsConfig config) {
            return ViewAttachments([=](auto& view) { return config; });
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
        This& ButtonView(BuildConfigFunc<ButtonViewConfig> buildConfigFunc);

        /// Adds a button view
        This& ButtonView(ButtonViewConfig config) {
            return ButtonView([=](auto& view) { return config; });
        }

        /// Adds a dynamic intermediate render view
        This& Immediate(BuildConfigFunc<ImmediateConfig> buildConfigFunc);

        /// Adds an intermediate render view
        This& Immediate(ImmediateConfig config) {
            return Immediate([=](auto& view) { return config; });
        }

        /// Adds an associate immediate render component for the current view
        This& AssociateImmediate(BuildConfigFunc<ImmediateConfig> buildConfigFunc);

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
        This& ToggleButtonView(BuildConfigFunc<ToggleButtonViewConfig> buildConfigFunc);

        /// Adds a toggle button view
        This& ToggleButtonView(ToggleButtonViewConfig config) {
            return ToggleButtonView([=](auto& view) { return config; });
        }

        /// Adds a slider
        This& SliderView(SliderViewConfig config);

        /// Adds a Spacer view
        This& Spacer();

        // MARK: View modifiers

        /// Adds a background view to the latest view
        This& Background(BuildViewFunc buildViewFunc);

        /// Adds an overlay view to the latest view
        This& Overlay(BuildViewFunc buildOverlayFunc);

        // MARK: Constraints

        /// Adds offset constraint
        This& Offset(float x = 0, float y = 0);

        /// Sets fixed size constraints for the most recent view
        This& FixedSize(std::optional<float> width, std::optional<float> height);

        /// Sets is ideal size for the most recent view
        This& SetIsIdealSize(bool isX = true, bool isY = true);

        // MARK: Design system components

        /// Adds a themed surface view
        This& Surface(SurfaceConfig config = {});

        /// Adds a themed button view
        This& Button(ButtonConfig config);

        /// Adds a themed segment toggle view
        This& SegmentToggle(ToggleButtonConfig config);

        /// Adds a themed image toggle view
        This& ImageToggle(ToggleButtonConfig config);

        /// Adds a themed progress bar view
        This& ProgressBar(ProgressBarConfig config);

        /// Adds a themed progress circle view
        This& ProgressCircle(ProgressBarConfig config);

        /// Adds a themed label view
        This& Label(LabelConfig config);

        /// Adds a themed check button view
        This& CheckButton(ToggleButtonConfig config);

        /// Adds a themed dial control view
        This& Dial(DialConfig config);

        /// Adds a themed radio button view
        This& RadioButton(ToggleButtonConfig config);

        /// Adds a slider view
        This& Slider(SliderConfig config);

        /// Adds a themed switch toggle button view
        This& SwitchToggle(ToggleButtonConfig config);

        /// Adds a themed toast view
        This& Toast(LabelConfig config);

        /// Adds a themed radio button group
        This& RadioButtonGroup(RadioButtonGroupConfig config);

        /// Adds a themed segmented picker control
        This& SegmentedPicker(SegmentedPickerConfig config);

        /// Adds a tool tip UI that will appear when the mouse is over the active view
        This& AddToolTip(ToolTipConfig config);

    protected:
        This& AddViewAttachments(View2D& view, ViewAttachmentsConfig config);
    };
} // namespace PJ

// Convenience macro
// TODO: rethink this. Doesn't work nicely with clangformat
#define VIEW(capture) .buildViewFunc = [capture](auto& vb)
#define COMMA ,
