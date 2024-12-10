#include "DuckDesignSystem.h"
#include "ImRenderer.h"
#include "QuickBuild.h"
#include "ToggleButtonControl.h"
#include "ViewBuilder.h"

using namespace std;
using namespace PJ;

// TODO: update when we have real fonts
DuckDesignSystem::DuckDesignSystem() :
    Base(Base::Config{}) {

    theme = MAKE<Theme>(Theme::Config{
        .colors = {
            { UIElementId::Surface, Color32(68, 68, 68) },
            { UIElementId::OnSurface, Color32(200, 200, 200) },
            { UIElementId::SurfaceContainer, Color32(61, 61, 61) },
            { UIElementId::SurfaceInteractive, Color32(83, 83, 83) },
            { UIElementId::SurfaceInteractivePress, Color32(120, 120, 120) },
            { UIElementId::SurfaceInteractiveHover, Color32(100, 100, 100) },
            { UIElementId::SurfaceToggleOff, Color32(50, 50, 50) },
            { UIElementId::SurfaceToggleOn, Color32(81, 113, 174) },
            { UIElementId::SurfaceToggleOnHover, Color32(107, 131, 188) },
            { UIElementId::SurfaceToggleOnPress, Color32(72, 92, 140) },
            { UIElementId::OnSurfaceInteractive, Color32(200, 200, 200) },
            { UIElementId::OnSurfaceInteractivePress, Color32(80, 80, 80) },
        },
        .elements = {
            { UIElementId::ControlFrame, Theme::ElementConfig{.size = Vector2{0, 30}, }},
            { UIElementId::SurfaceToggleOffPress, Theme::ElementConfig{.alias = UIElementId::SurfaceInteractivePress, }},
            { UIElementId::SurfaceToggleOffHover, Theme::ElementConfig{.alias = UIElementId::SurfaceInteractiveHover, }},
        }
    });

    buildViewFuncs[UIItemId::Surface] = [this](auto _config, auto& vb) {
        SurfaceConfig& config = *(static_cast<SurfaceConfig*>(_config));
        Color surfaceColor = Color::white;

        bool areShapesOpaque = this->areShapesOpaque;

        if (config.color) {
            surfaceColor = *config.color;
        } else if (!IsEmpty(config.colorId)) {
            surfaceColor = theme->ThemeColor(config.colorId, surfaceColor);
        }

        vb.Immediate({ .renderFunc = [=](auto& view, auto& renderer
                                     ) { renderer.FillRect(view.Bounds(), surfaceColor); },
                       .modifyRendererFunc = [=](auto& renderer
                                             ) { renderer.areShapesOpaque = areShapesOpaque; } });
    };

    buildViewFuncs[UIItemId::Button] = [this](auto _config, auto& vb) {
        ButtonConfig& config = *(static_cast<ButtonConfig*>(_config));

        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceInteractive, Color::gray);
        Color pressColor = theme->ThemeColor(UIElementId::SurfaceInteractivePress, Color::blue);
        Color hoverColor = theme->ThemeColor(UIElementId::SurfaceInteractiveHover, Color::red);
        Color labelColor = theme->ThemeColor(UIElementId::OnSurfaceInteractive, Color::black);
        Color labelPressColor =
            theme->ThemeColor(UIElementId::OnSurfaceInteractivePress, Color::white);
        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;

        bool areShapesOpaque = this->areShapesOpaque;

        vb.Button({ .onPressFunc = config.onPressFunc, .buildFrameFunc = [=](auto& vb) {
                       vb.Immediate({ .renderFunc =
                                          [=](auto& view, auto& renderer) {
                                              auto& button =
                                                  *(static_cast<ButtonControl*>(view.ParentView()));

                                              renderer.SetColor(
                                                  button.IsPressed()    ? pressColor
                                                  : button.IsHovering() ? hoverColor
                                                                        : surfaceColor
                                              );
                                              renderer.FillRect(view.Bounds());

                                              renderer.SetColor(
                                                  button.IsPressed() ? labelPressColor : labelColor
                                              );
                                              renderer.Text(config.label, { 0, 0 }, 32);
                                          },
                                      .modifyRendererFunc =
                                          [=](auto& renderer) {
                                              renderer.areShapesOpaque = areShapesOpaque;
                                              // Don't translate, so all contents are centered
                                              renderer.translateItemFunc = {};
                                          } }
                       ).FixedSize({}, frameHeight);
                   } });
    };

    buildViewFuncs[UIItemId::SegmentButton] = [this](auto _config, auto& vb) {
        ToggleButtonConfig& config = *(static_cast<ToggleButtonConfig*>(_config));

        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceToggleOff, Color::gray);
        Color pressColor = theme->ThemeColor(UIElementId::SurfaceToggleOffPress, Color::yellow);
        Color hoverColor = theme->ThemeColor(UIElementId::SurfaceToggleOffHover, Color::red);
        Color isOnColor = theme->ThemeColor(UIElementId::SurfaceToggleOn, Color::green);
        Color isOnHoverColor = theme->ThemeColor(UIElementId::SurfaceToggleOnHover, isOnColor);
        Color isOnPressColor = theme->ThemeColor(UIElementId::SurfaceToggleOnPress, isOnColor);
        Color labelColor = theme->ThemeColor(UIElementId::OnSurfaceInteractive, Color::black);
        Color labelPressColor =
            theme->ThemeColor(UIElementId::OnSurfaceInteractivePress, Color::white);
        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;

        bool areShapesOpaque = this->areShapesOpaque;

        vb.ToggleButton({ .isOnBinding = config.isOnBinding, .buildFrameFunc = [=](auto& vb) {
                             vb.Immediate(
                                   { .renderFunc =
                                         [=](auto& view, auto& renderer) {
                                             auto& button = *(static_cast<ToggleButtonControl*>(
                                                 view.ParentView()
                                             ));

                                             if (button.IsToggleOn()) {
                                                 renderer.SetColor(
                                                     button.IsPressed()    ? isOnPressColor
                                                     : button.IsHovering() ? isOnHoverColor
                                                                           : isOnColor
                                                 );
                                             } else {
                                                 renderer.SetColor(
                                                     button.IsPressed()    ? pressColor
                                                     : button.IsHovering() ? hoverColor
                                                                           : surfaceColor
                                                 );
                                             }
                                             renderer.FillRect(view.Bounds());

                                             renderer.SetColor(
                                                 button.IsPressed() ? labelPressColor : labelColor
                                             );
                                             renderer.Text(config.label, { 0, 0 }, 32);
                                         },
                                     .modifyRendererFunc =
                                         [=](auto& renderer) {
                                             renderer.areShapesOpaque = areShapesOpaque;
                                             // Don't translate, so all contents are centered
                                             renderer.translateItemFunc = {};
                                         } }
                             ).FixedSize({}, frameHeight);
                         } });
    };

    buildViewFuncs[UIItemId::CheckButton] = [this](auto _config, auto& vb) {
        ToggleButtonConfig& config = *(static_cast<ToggleButtonConfig*>(_config));

        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceToggleOff, Color::gray);
        Color pressColor = theme->ThemeColor(UIElementId::SurfaceToggleOffPress, Color::yellow);
        Color hoverColor = theme->ThemeColor(UIElementId::SurfaceToggleOffHover, Color::red);
        Color isOnColor = theme->ThemeColor(UIElementId::SurfaceToggleOn, Color::green);
        Color isOnHoverColor = theme->ThemeColor(UIElementId::SurfaceToggleOnHover, isOnColor);
        Color isOnPressColor = theme->ThemeColor(UIElementId::SurfaceToggleOnPress, isOnColor);
        Color labelColor = theme->ThemeColor(UIElementId::OnSurface, Color::black);
        Color labelPressColor = labelColor;
        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;

        bool areShapesOpaque = this->areShapesOpaque;

        vb.ToggleButton({ .isOnBinding = config.isOnBinding, .buildFrameFunc = [=](auto& vb) {
                             vb.Immediate(
                                   { .renderFunc =
                                         [=](auto& view, auto& renderer) {
                                             auto& button = *(static_cast<ToggleButtonControl*>(
                                                 view.ParentView()
                                             ));

                                             if (button.IsToggleOn()) {
                                                 renderer.SetColor(
                                                     button.IsPressed()    ? isOnPressColor
                                                     : button.IsHovering() ? isOnHoverColor
                                                                           : isOnColor
                                                 );
                                             } else {
                                                 renderer.SetColor(
                                                     button.IsPressed()    ? pressColor
                                                     : button.IsHovering() ? hoverColor
                                                                           : surfaceColor
                                                 );
                                             }

                                             Rect checkFrame = view.Bounds();
                                             checkFrame.size.x = checkFrame.size.y;
                                             renderer.FillRect(checkFrame);

                                             renderer.SetColor(
                                                 button.IsPressed() ? labelPressColor : labelColor
                                             );
                                             renderer.Text(
                                                 config.label, { checkFrame.size.x + 15, 0 }, 32
                                             );
                                         },
                                     .modifyRendererFunc =
                                         [=](auto& renderer) {
                                             renderer.areShapesOpaque = areShapesOpaque;
                                         } }
                             ).FixedSize({}, frameHeight);
                         } });
    };

    buildViewFuncs[UIItemId::ProgressBar] = [this](auto _config, auto& vb) {
        ProgressBarConfig& config = *(static_cast<ProgressBarConfig*>(_config));

        Color backColor = theme->ThemeColor(UIElementId::SecondaryContainer, Color::gray);
        Color barColor =
            config.color ? *config.color : theme->ThemeColor(UIElementId::Primary, Color::black);
        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;

        bool areShapesOpaque = this->areShapesOpaque;

        vb.Immediate({ .renderFunc =
                           [=](auto& view, auto& renderer) {
                               renderer.FillRect(view.Bounds(), backColor);

                               if (config.valueFunc) {
                                   float value = config.valueFunc();

                                   Rect barBounds = view.Bounds();
                                   barBounds.size.x *= value;

                                   renderer.FillRect(barBounds, barColor);
                               }
                           },
                       .modifyRendererFunc = [=](auto& renderer
                                             ) { renderer.areShapesOpaque = areShapesOpaque; } }
        ).FixedSize({}, frameHeight);
    };

    buildViewFuncs[UIItemId::Label] = [this](auto _config, auto& vb) {
        LabelConfig& config = *(static_cast<LabelConfig*>(_config));

        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;
        Color color;

        if (config.color) {
            color = *config.color;
        } else if (!IsEmpty(config.colorId)) {
            color = theme->ThemeColor(config.colorId, color);
        }

        vb.Immediate({ .renderFunc = [=](auto& view, auto& renderer
                                     ) { renderer.Text(config.text, { 0, 0 }, 32, color); } }
        ).FixedSize({}, frameHeight);
    };

    buildViewFuncs[UIItemId::Toast] = [this](auto _config, auto& vb) {
        LabelConfig& config = *(static_cast<LabelConfig*>(_config));

        // TODO: update colors
        Color surfaceColor = Color::yellow;
        Color onSurfaceColor = Color::white;

        vb.MatchZStack({ .buildBackgroundFunc =
                             [=](auto& vb) {
                                 vb.Immediate({ .renderFunc = [=](auto& view, auto& renderer) {
                                     renderer.FillRect(view.Bounds(), surfaceColor);
                                 } });
                             },
                         .buildViewFunc =
                             [=](auto& vb) {
                                 vb.Pad({ .insets = LayoutInsets::Uniform(32),
                                          .buildViewFunc = [=](auto& vb) {
                                              vb.Text({ .text = config.text });
                                          } });
                             } });
    };
}
