#include "DuckDesignSystem.h"
#include "DialControl.h"
#include "ImRenderer.h"
#include "QuickBuilder.h"
#include "SliderControl.h"
#include "ToggleButtonControl.h"
#include "ViewBuilder.h"

using namespace std;
using namespace PJ;

// FUTURE: update when we have real fonts
DuckDesignSystem::DuckDesignSystem() :
    Base(Base::Config{}) {

    theme = MAKE<Theme>(Theme::Config{
        .colors = {
            { UIElementId::Primary, Color32(120, 105, 166) },
            { UIElementId::OnPrimary, Color::white },
            { UIElementId::PrimaryContainer, Color32(196, 180, 248) },
            { UIElementId::OnPrimaryContainer, Color32(75, 60, 119) },
            { UIElementId::SecondaryContainer, Color32(225, 224, 247) },
            { UIElementId::OnSecondaryContainer, Color32(68, 69, 87) },
            
            { UIElementId::Surface, Color32(78, 78, 88) },
            { UIElementId::OnSurface, Color32(200, 200, 200) },
            { UIElementId::Secondary, Color32(93, 93, 112) },
            { UIElementId::OnSecondary, Color32(255, 255, 255) },
            
            { UIElementId::SurfaceContainer, Color32(40, 40, 40) },
            { UIElementId::OnSurfaceContainer, Color32(215, 215, 215) },
            { UIElementId::SurfaceInteractive, Color32(83, 83, 83) },
            { UIElementId::SurfaceInteractivePress, Color32(120, 120, 120) },
            { UIElementId::SurfaceInteractiveHover, Color32(100, 100, 100) },
            { UIElementId::SurfaceToggleOff, Color32(50, 50, 50) },
            { UIElementId::SurfaceToggleOn, Color32(81, 113, 174) },
            { UIElementId::SurfaceToggleOnHover, Color32(107, 131, 188) },
            { UIElementId::SurfaceToggleOnPress, Color32(72, 92, 140) },
            { UIElementId::OnSurfaceInteractive, Color32(200, 200, 200) },
            { UIElementId::OnSurfaceInteractivePress, Color32(80, 80, 80) },
            
            { UIElementId::SurfaceContainerLowSecondary, Color32(35, 35, 35) },
            { UIElementId::SurfaceContainerLowTertiary, Color32(15, 15, 15) },
            { UIElementId::SurfaceContainerHighSecondary, Color32(50, 50, 50) },
            { UIElementId::SurfaceContainerHighTertiary, Color32(65, 65, 65) },
        },
        .elements = {
            { UIElementId::ControlFrame, Theme::ElementConfig{.size = Vector2{0, 30}, }},
            { UIElementId::SurfaceToggleOffPress, Theme::ElementConfig{.alias = UIElementId::SurfaceInteractivePress, }},
            { UIElementId::SurfaceToggleOffHover, Theme::ElementConfig{.alias = UIElementId::SurfaceInteractiveHover, }},
            { UIElementId::OnSurfaceContainerLowSecondary, Theme::ElementConfig{.alias = UIElementId::OnSurfaceContainer, }},
            { UIElementId::OnSurfaceContainerLowTertiary, Theme::ElementConfig{.alias = UIElementId::OnSurfaceContainer, }},
            { UIElementId::OnSurfaceContainerHighSecondary, Theme::ElementConfig{.alias = UIElementId::OnSurfaceContainer, }},
            { UIElementId::OnSurfaceContainerHighTertiary, Theme::ElementConfig{.alias = UIElementId::OnSurfaceContainer, }},
        }
    });

    // MARK: Surface

    buildViewFuncs[UIItemId::Surface] = [this](auto* _config, auto& vb) {
        SurfaceConfig const& config = *(static_cast<SurfaceConfig const*>(_config));
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

    // MARK: Button

    buildViewFuncs[UIItemId::Button] = [this](auto* _config, auto& vb) {
        ButtonConfig const& config = *(static_cast<ButtonConfig const*>(_config));

        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceInteractive, Color::gray);
        Color pressColor = theme->ThemeColor(UIElementId::SurfaceInteractivePress, Color::blue);
        Color hoverColor = theme->ThemeColor(UIElementId::SurfaceInteractiveHover, Color::red);
        Color labelColor = theme->ThemeColor(UIElementId::OnSurfaceInteractive, Color::black);
        Color labelPressColor =
            theme->ThemeColor(UIElementId::OnSurfaceInteractivePress, Color::white);
        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;

        bool areShapesOpaque = this->areShapesOpaque;

        vb.ButtonView({ .onPressFunc = config.onPressFunc,
                        .modifyViewFunc = config.modifyViewFunc,
                        .buildFrameFunc = [=](auto& vb) {
                            vb.Immediate(
                                  { .renderFunc =
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

    // MARK: Slider

    buildViewFuncs[UIItemId::Slider] = [this](auto* _config, auto& vb) {
        SliderConfig const& config = *(static_cast<SliderConfig const*>(_config));

        Color trackColor = theme->ThemeColor(UIElementId::SurfaceInteractive, Color::gray);
        Color trackPrimaryColor = theme->ThemeColor(UIElementId::PrimaryContainer, Color::gray);
        Color trackHoverColor =
            theme->ThemeColor(UIElementId::SurfaceInteractiveHover, Color::gray);
        Color thumbColor = theme->ThemeColor(UIElementId::Primary, Color::black);
        Color thumbPressColor =
            theme->ThemeColor(UIElementId::SurfaceInteractivePress, Color::blue);

        float frameOrthogonal = theme->ElementSize(UIElementId::ControlFrame, { 30, 30 })
                                    .AxisValueOrthogonal(config.axis);

        bool areShapesOpaque = this->areShapesOpaque;

        vb.QB()
            .And("Slider")
            .template With<SliderControl>(SliderControl::Config{ .axis = config.axis,
                                                                 .valueBinding =
                                                                     config.valueBinding,
                                                                 .trackOrthogonal = frameOrthogonal,
                                                                 .minValue = config.minValue,
                                                                 .maxValue = config.maxValue })
            .template With<ImRenderer>(ImRenderer::Config{ .areShapesOpaque = areShapesOpaque })
            .template ModifyLatest<ImRenderer>([=](auto& renderer) {
                renderer.AddSignalHandler(
                    { .id = SignalId::RenderPrepare,
                      .func =
                          [=,
                           slider = (SliderControl*)nullptr](auto& renderer, auto& signal) mutable {
                              ImRenderer& imRenderer = *(static_cast<ImRenderer*>(&renderer));

                              if (nullptr == slider) {
                                  slider = renderer.Node()->template TypeComponent<SliderControl>();
                              }
                              GUARD(slider);

                              Rect trackBounds = slider->Bounds();
                              imRenderer.FillRect(
                                  trackBounds, slider->IsHovering() ? trackHoverColor : trackColor
                              );

                              Vector2 thumbSize{ frameOrthogonal, frameOrthogonal };
                              float value = slider->NormalValue();

                              float sliderAxis = slider->Bounds().size.AxisValue(config.axis);
                              float minThumbPos = 0;
                              float maxThumbPos = sliderAxis - frameOrthogonal;
                              Vector2 thumbOrigin;

                              // FUTURE: support vertical slider if needed

                              thumbOrigin.AxisValue(config.axis) =
                                  minThumbPos + (maxThumbPos - minThumbPos) * value;

                              trackBounds.size.AxisValue(config.axis) =
                                  thumbOrigin.AxisValue(config.axis);
                              imRenderer.FillRect(trackBounds, trackPrimaryColor);

                              Rect thumbRect{ .origin = thumbOrigin, .size = thumbSize };
                              imRenderer.FillRect(
                                  thumbRect, slider->IsDragging() ? thumbPressColor : thumbColor
                              );
                          } }
                );
            });
    };

    // MARK: Collapsing Header

    // Need mechanism for updating views when values change
    //    buildViewFuncs[UIItemId::CollapsingHeader] = [this](auto _config, auto& vb) {
    //        CollapsingHeaderConfig& config = *(static_cast<CollapsingHeaderConfig*>(_config));
    //
    //        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceInteractive, Color::gray);
    //        Color pressColor = theme->ThemeColor(UIElementId::SurfaceInteractivePress,
    //        Color::blue); Color hoverColor =
    //        theme->ThemeColor(UIElementId::SurfaceInteractiveHover, Color::red); Color labelColor
    //        = theme->ThemeColor(UIElementId::OnSurfaceInteractive, Color::black); Color
    //        labelPressColor =
    //            theme->ThemeColor(UIElementId::OnSurfaceInteractivePress, Color::white);
    //        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;
    //
    //        bool areShapesOpaque = this->areShapesOpaque;
    //            vb.ButtonView({ .onPressFunc = [=](auto& button) {
    //                config.isCollapsedBinding.SetValue(!config.isCollapsedBinding.Value());
    //            }, .buildFrameFunc = [=](auto& vb) {
    //                           vb.Immediate(
    //                                 { .renderFunc =
    //                                       [=](auto& view, auto& renderer) {
    //                                           auto& button =
    //                                               *(static_cast<ButtonControl*>(view.ParentView()));
    //
    //                                           renderer.SetColor(
    //                                               button.IsPressed()    ? pressColor
    //                                               : button.IsHovering() ? hoverColor
    //                                                                     : surfaceColor
    //                                           );
    //                                           renderer.FillRect(view.Bounds());
    //
    //                                           renderer.SetColor(
    //                                               button.IsPressed() ? labelPressColor :
    //                                               labelColor
    //                                           );
    //                                           renderer.Text(config.label, { 0, 0 }, 32);
    //                                       },
    //                                   .modifyRendererFunc =
    //                                       [=](auto& renderer) {
    //                                           renderer.areShapesOpaque = areShapesOpaque;
    //                                           // Don't translate, so all contents are centered
    //                                           renderer.translateItemFunc = {};
    //                                       } }
    //                           ).FixedSize({}, frameHeight);
    //                       } });
    //    };

    // MARK: Segment toggle

    buildViewFuncs[UIItemId::SegmentToggle] = [this](auto* _config, auto& vb) {
        ToggleButtonConfig const& config = *(static_cast<ToggleButtonConfig const*>(_config));

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

        vb.ToggleButtonView({ .isOnBinding = config.isOnBinding,
                              .modifyViewFunc = config.modifyViewFunc,
                              .buildFrameFunc = [=](auto& vb) {
                                  vb.Immediate(
                                        { .renderFunc =
                                              [=](auto& view, auto& renderer) {
                                                  auto& button =
                                                      *(static_cast<ToggleButtonControl*>(
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
                                                      button.IsPressed() ? labelPressColor
                                                                         : labelColor
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

    //    auto setToggleColor = [this](ToggleButtonControl& button, ImRenderer& renderer) {
    //        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceToggleOff, Color::gray);
    //        Color pressColor = theme->ThemeColor(UIElementId::SurfaceToggleOffPress,
    //        Color::yellow); Color hoverColor =
    //        theme->ThemeColor(UIElementId::SurfaceToggleOffHover, Color::red); Color isOnColor =
    //        theme->ThemeColor(UIElementId::SurfaceToggleOn, Color::green); Color isOnHoverColor =
    //        theme->ThemeColor(UIElementId::SurfaceToggleOnHover, isOnColor); Color isOnPressColor
    //        = theme->ThemeColor(UIElementId::SurfaceToggleOnPress, isOnColor);
    //
    //         if (button.IsToggleOn()) {
    //             renderer.SetColor(
    //                 button.IsPressed()    ? isOnPressColor
    //                 : button.IsHovering() ? isOnHoverColor
    //                                       : isOnColor
    //             );
    //         } else {
    //             renderer.SetColor(
    //                 button.IsPressed()    ? pressColor
    //                 : button.IsHovering() ? hoverColor
    //                                       : surfaceColor
    //             );
    //         }
    //    };

    // MARK: ImageToggle

    buildViewFuncs[UIItemId::ImageToggle] = [this](auto* _config, auto& vb) {
        ToggleButtonConfig const& config = *(static_cast<ToggleButtonConfig const*>(_config));

        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceToggleOff, Color::gray);
        Color pressColor = theme->ThemeColor(UIElementId::SurfaceToggleOffPress, Color::yellow);
        Color hoverColor = theme->ThemeColor(UIElementId::SurfaceToggleOffHover, Color::red);
        Color isOnColor = theme->ThemeColor(UIElementId::SurfaceToggleOn, Color::green);
        Color isOnHoverColor = theme->ThemeColor(UIElementId::SurfaceToggleOnHover, isOnColor);
        Color isOnPressColor = theme->ThemeColor(UIElementId::SurfaceToggleOnPress, isOnColor);
        Color imageColor = config.imageColor
                               ? *config.imageColor
                               : theme->ThemeColor(UIElementId::OnSurfaceInteractive, Color::white);

        bool areShapesOpaque = this->areShapesOpaque;

        float frameWidth = config.size ? config.size->x : 60;
        float frameHeight = config.size ? config.size->y : 60;

        vb.ToggleButtonView(
            { .isOnBinding = config.isOnBinding,
              .modifyViewFunc = config.modifyViewFunc,
              .buildFrameFunc =
                  [=](auto& vb) {
                      vb.Immediate(
                            { .renderFunc =
                                  [=](auto& view, auto& renderer) {
                                      auto& button =
                                          *(static_cast<ToggleButtonControl*>(view.ParentView()));

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
                                      renderer.FillRoundRect(view.Bounds(), 10);
                                      renderer.TemplateImage(config.imageId, { 0, 0 }, imageColor);
                                  },
                              .modifyRendererFunc =
                                  [=](auto& renderer) {
                                      renderer.areShapesOpaque = areShapesOpaque;
                                      // Don't translate, so all contents are centered
                                      renderer.translateItemFunc = {};
                                  } }
                      ).FixedSize(frameWidth, frameHeight);
                  } }
        );
    };

    // MARK: SwitchToggle

    buildViewFuncs[UIItemId::SwitchToggle] = [this](auto* _config, auto& vb) {
        ToggleButtonConfig const& config = *(static_cast<ToggleButtonConfig const*>(_config));

        Color primaryColor = theme->ThemeColor(UIElementId::Primary, Color::gray);
        Color pressColor = theme->ThemeColor(UIElementId::SurfaceInteractivePress, Color::yellow);
        Color hoverColor = theme->ThemeColor(UIElementId::SurfaceInteractiveHover, Color::red);

        Color primaryContainerColor = theme->ThemeColor(UIElementId::PrimaryContainer, Color::gray);
        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceToggleOff, Color::gray);

        bool areShapesOpaque = this->areShapesOpaque;

        float frameWidth = config.size ? config.size->x : 60;
        float frameHeight = config.size ? config.size->y : 30;

        vb.ToggleButtonView(
            { .isOnBinding = config.isOnBinding,
              .modifyViewFunc = config.modifyViewFunc,
              .buildFrameFunc =
                  [=](auto& vb) {
                      vb.Immediate(
                            { .renderFunc =
                                  [=](auto& view, auto& renderer) {
                                      auto& button =
                                          *(static_cast<ToggleButtonControl*>(view.ParentView()));

                                      renderer.SetColor(
                                          button.IsToggleOn() ? primaryContainerColor : surfaceColor
                                      );
                                      renderer.FillCapsule(view.Bounds());

                                      Rect circleBounds;
                                      circleBounds.size = { frameHeight, frameHeight };

                                      if (button.IsToggleOn()) {
                                          circleBounds.origin.x = frameWidth - circleBounds.size.x;
                                      }

                                      renderer.SetColor(
                                          button.IsPressed()    ? pressColor
                                          : button.IsHovering() ? hoverColor
                                                                : primaryColor
                                      );
                                      renderer.FillEllipse(circleBounds);
                                  },
                              .modifyRendererFunc =
                                  [=](auto& renderer) {
                                      renderer.areShapesOpaque = areShapesOpaque;
                                  } }
                      ).FixedSize(frameWidth, frameHeight);
                  } }
        );
    };

    // MARK: buildCheckTypeButtonFunc

    auto buildCheckTypeButtonFunc = [this](
                                        auto* _config, ViewBuilder& vb,
                                        std::function<void(ImRenderer&, Rect)> drawFill,
                                        std::function<void(ImRenderer&, Rect, float)> drawFrame
                                    ) {
        ToggleButtonConfig const& config = *(static_cast<ToggleButtonConfig const*>(_config));

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

        vb.ToggleButtonView(
            { .isOnBinding = config.isOnBinding,
              .modifyViewFunc = config.modifyViewFunc,
              .buildFrameFunc =
                  [=](auto& vb) {
                      vb.Immediate(
                            { .renderFunc =
                                  [=](auto& view, auto& renderer) {
                                      auto& button =
                                          *(static_cast<ToggleButtonControl*>(view.ParentView()));

                                      // FUTURE: support outline role (see Material)
                                      float strokeWidth = button.IsPressed() ? 5 : 3;

                                      Color color;
                                      if (button.IsToggleOn()) {
                                          color = button.IsPressed()    ? isOnPressColor
                                                  : button.IsHovering() ? isOnHoverColor
                                                                        : isOnColor;
                                      } else {
                                          color = button.IsPressed()    ? pressColor
                                                  : button.IsHovering() ? hoverColor
                                                                        : surfaceColor;
                                      }
                                      renderer.SetColor(color);
                                      renderer.SetStrokeColor(color);

                                      Rect checkFrame = view.Bounds();
                                      checkFrame.size.x = checkFrame.size.y;

                                      if (button.IsToggleOn()) {
                                          drawFill(renderer, checkFrame);
                                      } else {
                                          drawFrame(renderer, checkFrame, strokeWidth);
                                      }

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
                  } }
        );
    };

    // MARK: Check button

    buildViewFuncs[UIItemId::CheckButton] = [buildCheckTypeButtonFunc,
                                             this](auto* _config, auto& vb) {
        buildCheckTypeButtonFunc(
            _config, vb, [](auto& renderer, auto frame) { renderer.FillRect(frame); },
            [](auto& renderer, auto frame, auto strokeWidth) {
                renderer.SetStrokeWidth(strokeWidth);
                renderer.FrameRect(frame);
            }
        );
    };

    // MARK: Radio button

    buildViewFuncs[UIItemId::RadioButton] = [buildCheckTypeButtonFunc,
                                             this](auto* _config, auto& vb) {
        buildCheckTypeButtonFunc(
            _config, vb, [](auto& renderer, auto frame) { renderer.FillEllipse(frame); },
            [](auto& renderer, auto frame, auto strokeWidth) {
                renderer.SetStrokeWidth(strokeWidth);
                renderer.FrameEllipse(frame);
            }
        );
    };

    // MARK: Progress bar

    buildViewFuncs[UIItemId::ProgressBar] = [this](auto* _config, auto& vb) {
        ProgressBarConfig const& config = *(static_cast<ProgressBarConfig const*>(_config));

        Color backColor = theme->ThemeColor(UIElementId::Secondary, Color::gray);
        Color barColor = config.progressColor
                             ? *config.progressColor
                             : theme->ThemeColor(UIElementId::Primary, Color::black);
        float frameHeight = theme->ElementSize(UIElementId::ControlFrame, { 0, 30 }).y;

        bool areShapesOpaque = this->areShapesOpaque;

        vb.Immediate({ .renderFunc =
                           [=](auto& view, auto& renderer) {
                               renderer.FillCapsule(view.Bounds(), Axis2D::X, backColor);

                               if (config.valueFunc) {
                                   float value = clamp(config.valueFunc(), 0.0f, 1.0f);

                                   Rect barBounds = view.Bounds();
                                   barBounds.size.x *= value;

                                   renderer.FillCapsule(barBounds, Axis2D::X, barColor);
                               }
                           },
                       .modifyRendererFunc = [=](auto& renderer
                                             ) { renderer.areShapesOpaque = areShapesOpaque; } }
        ).FixedSize({}, frameHeight);
    };

    // MARK: Progress Circle

    buildViewFuncs[UIItemId::ProgressCircle] = [this](auto* _config, auto& vb) {
        ProgressBarConfig const& config = *(static_cast<ProgressBarConfig const*>(_config));

        Color backColor = theme->ThemeColor(UIElementId::Secondary, Color::gray);
        Color barColor = config.progressColor
                             ? *config.progressColor
                             : theme->ThemeColor(UIElementId::Primary, Color::black);

        bool areShapesOpaque = this->areShapesOpaque;

        vb.Immediate({ .renderFunc =
                           [=](auto& view, auto& renderer) {
                               float strokeWidth = 10;
                               renderer.SetStrokeWidth(strokeWidth);

                               float radius =
                                   std::min(view.Bounds().size.x, view.Bounds().size.y) / 2.0f;
                               Vector2 circleCenter{ view.Bounds().size.x / 2.0f,
                                                     view.Bounds().size.y / 2.0f };
                               renderer.FrameCircle(circleCenter, radius, backColor);

                               if (config.valueFunc) {
                                   float value = clamp(config.valueFunc(), 0.0f, 1.0f);

                                   Vector2 arcSize{ radius * 2, radius * 2 };
                                   Rect arcFrame{ .origin = { circleCenter.x - radius,
                                                              circleCenter.y - radius },
                                                  .size = arcSize };
                                   Angle angleDelta = Angle::WithDegrees(360.0f * value);
                                   renderer.FrameArc(arcFrame, Angle{}, angleDelta, barColor);
                               }
                           },
                       .modifyRendererFunc = [=](auto& renderer
                                             ) { renderer.areShapesOpaque = areShapesOpaque; } }
        ).FixedSize(60, 60);
    };

    // MARK: Dial

    buildViewFuncs[UIItemId::Dial] = [this](auto* _config, auto& vb) {
        DialConfig const& config = *(static_cast<DialConfig const*>(_config));

        Color surfaceColor = config.surfaceColor
                                 ? *config.surfaceColor
                                 : theme->ThemeColor(UIElementId::Primary, Color::gray);
        Color onSurfaceColor = config.onSurfaceColor
                                   ? *config.onSurfaceColor
                                   : theme->ThemeColor(UIElementId::OnPrimary, Color::blue);

        bool areShapesOpaque = this->areShapesOpaque;
        float radius = 30;
        Vector2 frameSize{ radius * 2, radius * 2 };

        vb.QB().And("Dial").template With<DialControl>(DialControl::Config{
            .radius = radius, .valueBinding = config.valueBinding });

        ViewBuilder::ImmediateConfig immediateConfig{
            .renderFunc =
                [=](auto& view, auto& renderer) {
                    float radius = std::min(view.Bounds().size.x, view.Bounds().size.y) / 2.0f;
                    Vector2 circleCenter{ view.Bounds().size.x / 2.0f,
                                          view.Bounds().size.y / 2.0f };
                    renderer.FillCircle(circleCenter, radius, surfaceColor);

                    float value = clamp(config.valueBinding.Value(), 0.0f, 1.0f);

                    float minAngle = -150;
                    float maxAngle = 150;
                    float angle = minAngle + (maxAngle - minAngle) * value;

                    Vector2 start = circleCenter;
                    Vector2 end = start + Angle::WithDegrees(angle).ToVector2() * radius;
                    renderer.SetStartPathCap(PathCap::Flat)
                        .SetEndPathCap(PathCap::Flat)
                        .SetStrokeWidth(8)
                        .SetStrokeColor(onSurfaceColor);

                    renderer.Line(start, end);
                },
            .modifyRendererFunc = [=](auto& renderer
                                  ) { renderer.areShapesOpaque = areShapesOpaque; }
        };

        vb.AssociateImmediate([=](auto& view) { return immediateConfig; }
        ).FixedSize(frameSize.x, frameSize.y);

        vb.QB().Pop();
    };

    // MARK: Label

    buildViewFuncs[UIItemId::Label] = [this](auto* _config, auto& vb) {
        LabelConfig const& config = *(static_cast<LabelConfig const*>(_config));

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

    // MARK: Toast

    buildViewFuncs[UIItemId::Toast] = [this](auto* _config, auto& vb) {
        LabelConfig const& config = *(static_cast<LabelConfig const*>(_config));

        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceContainerHighTertiary);

        vb.Pad({ .insets = LayoutInsets::Uniform(32),
                 .buildViewFunc = [=](auto& vb) { vb.Text({ .text = config.text }); } }
        ).Background([=](auto& vb) {
            vb.Immediate({ .renderFunc = [=](auto& view, auto& renderer) {
                renderer.FillRect(view.Bounds(), surfaceColor);
            } });
        });
    };

    // MARK: ToolTip

    buildViewFuncs[UIItemId::ToolTip] = [this](auto* _config, auto& vb) {
        ToolTipConfig const& config = *(static_cast<ToolTipConfig const*>(_config));

        Color surfaceColor = theme->ThemeColor(UIElementId::SurfaceContainerLowTertiary);

        BuildViewFunc buildBackgroundFunc = [=](auto& vb) {
            vb.Immediate({ .renderFunc = [=](auto& view, auto& renderer) {
                renderer.FillRect(view.Bounds(), surfaceColor);
            } });
        };

        BuildViewFunc buildViewFunc = [=](auto& vb) {
            vb.Pad({ .insets = LayoutInsets::Uniform(16),
                     .buildViewFunc = [=](auto& vb) { vb.Text({ .text = config.text }); } });
        };

        vb.ViewAttachments({ .buildBackgroundFunc = buildBackgroundFunc,
                             .buildViewFunc = buildViewFunc,
                             .modifyViewFunc = [config](auto& view) {
                                 GUARD(config.result);
                                 *config.result = &view;
                             } });
    };
}
