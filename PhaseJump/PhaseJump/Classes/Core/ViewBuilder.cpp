#include "ViewBuilder.h"
#include "Collider2D.h"
#include "ColorView.h"
#include "FixedGridViewLayout.h"
#include "FlowStackViewLayout.h"
#include "Font.h"
#include "HoverGestureHandler.h"
#include "ImRenderer.h"
#include "PadViewLayout.h"
#include "QuickBuilder.h"
#include "SliderControl.h"
#include "SpacerView.h"
#include "TextRenderer.h"
#include "TextView.h"
#include "ToggleButtonControl.h"
#include "View2D.h"
#include "ViewAttachmentsLayout.h"
#include "World.h"
#include "ZStackViewLayout.h"

using namespace std;
using namespace PJ;

using This = ViewBuilder;

ViewBuilder::ViewBuilder(QuickBuilder& quickBuilder) :
    quickBuilder(&quickBuilder) {}

ViewBuilder::ViewBuilder(WorldNode& node) {
    qb = NEW<QuickBuilder>(node);
}

This& ViewBuilder::Pad(BuildConfigFunc<PadConfig> buildConfigFunc) {
    QB().And()
        .With<View2D>()
        .ModifyLatest<View2D>([=](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                UP<SomeViewLayout> layout = NEW<PadViewLayout>(config.insets);
                view.SetLayout(layout);

                GUARD(config.buildViewFunc)

                ViewBuilder viewBuilder(*view.owner);
                config.buildViewFunc(viewBuilder);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::HStack(BuildConfigFunc<HStackConfig> buildConfigFunc) {
    QB().And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                UP<SomeViewLayout> layout =
                    NEW<FlowStackViewLayout>(Axis2D::X, config.spacing, config.alignFunc);
                view.SetLayout(layout);

                GUARD(config.buildViewFunc)

                ViewBuilder viewBuilder(*view.owner);
                config.buildViewFunc(viewBuilder);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::VStack(BuildConfigFunc<VStackConfig> buildConfigFunc) {
    QB().And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                UP<SomeViewLayout> layout =
                    NEW<FlowStackViewLayout>(Axis2D::Y, config.spacing, config.alignFunc);
                view.SetLayout(layout);

                GUARD(config.buildViewFunc)

                ViewBuilder viewBuilder(*view.owner);
                config.buildViewFunc(viewBuilder);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::FixedGrid(BuildConfigFunc<FixedGridConfig> buildConfigFunc) {
    QB().And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                UP<SomeViewLayout> layout = NEW<FixedGridViewLayout>(FixedGridViewLayout::Config{
                    .gridSize = config.gridSize,
                    .cellSize = config.cellSize,
                    .alignment = config.alignment,
                    .spacing = config.spacing });
                view.SetLayout(layout);

                GUARD(config.buildViewFunc)

                ViewBuilder viewBuilder(*view.owner);
                config.buildViewFunc(viewBuilder);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::ZStack(BuildConfigFunc<ZStackConfig> buildConfigFunc) {
    QB().And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                UP<SomeViewLayout> layout =
                    NEW<ZStackViewLayout>(config.alignment.xAlignFunc, config.alignment.yAlignFunc);
                view.SetLayout(layout);

                GUARD(config.buildViewFunc)

                ViewBuilder viewBuilder(*view.owner);
                config.buildViewFunc(viewBuilder);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::Background(BuildViewFunc buildBackgroundFunc) {
    QB().ModifyLatest<View2D>([&](auto& activeView) {
        auto parentView = activeView.ParentView();
        GUARD_LOG(parentView, "ERROR: Missing parent view for background")

        auto viewNode = parentView->owner->Move(*activeView.owner);

        ViewBuilder vb(*parentView->owner);
        vb.ViewAttachments([=](auto& view) {
            return ViewAttachmentsConfig{ .buildBackgroundFunc = buildBackgroundFunc,
                                          .viewNode = viewNode };
        });
    });

    return *this;
}

This& ViewBuilder::Overlay(BuildViewFunc buildOverlayFunc) {
    // Important: Add Background first or the view order will be wrong
    // FUTURE: make this more intelligent if needed
    QB().ModifyLatest<View2D>([&](auto& activeView) {
        auto parentView = activeView.ParentView();
        GUARD_LOG(parentView, "ERROR: Missing parent view for background")

        auto viewNode = parentView->owner->Move(*activeView.owner);

        ViewBuilder vb(*parentView->owner);
        vb.ViewAttachments([=](auto& view) {
            return ViewAttachmentsConfig{ .buildOverlayFunc = buildOverlayFunc,
                                          .viewNode = viewNode };
        });
    });

    return *this;
}

This& ViewBuilder::ViewAttachments(BuildConfigFunc<ViewAttachmentsConfig> buildConfigFunc) {
    QB().And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                ViewBuilder vb(*view.owner);
                vb.AddViewAttachments(view, config);

                if (config.modifyViewFunc) {
                    config.modifyViewFunc(view);
                }
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::AddViewAttachments(View2D& view, ViewAttachmentsConfig config) {
    UP<SomeViewLayout> layout = NEW<ViewAttachmentsLayout>();
    view.SetLayout(layout);

    ViewBuilder backgroundViewBuilder(QB().Node());
    ViewBuilder viewBuilder(QB().Node());
    ViewBuilder overlayViewBuilder(QB().Node());

    if (config.buildBackgroundFunc) {
        config.buildBackgroundFunc(backgroundViewBuilder);
        auto childViews = view.ChildViews();
        if (!IsEmpty(childViews)) {
            childViews[childViews.size() - 1]->isAttachment = true;
        }
    }

    if (config.viewNode) {
        QB().Node().Add(config.viewNode);
    } else if (config.buildViewFunc) {
        config.buildViewFunc(viewBuilder);
    }

    if (config.buildOverlayFunc) {
        config.buildOverlayFunc(overlayViewBuilder);
        auto childViews = view.ChildViews();
        if (!IsEmpty(childViews)) {
            childViews[childViews.size() - 1]->isAttachment = true;
        }
    }

    QB().Pop();

    return *this;
}

This& ViewBuilder::Color(BuildConfigFunc<ColorConfig> buildConfigFunc) {
    QB().And("")
        .With<ColorView>()
        .ModifyLatest<ColorView>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                static_cast<ColorView*>(&view)->SetColor(config.color);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::Text(BuildConfigFunc<TextConfig> buildConfigFunc) {
    QB().And("")
        .With<TextView>()
        .ModifyLatest<TextView>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                auto world = view.owner->World();
                GUARD(world)

                auto font = FindFont(world->resources, config.fontSpec);
                GUARD_LOG(font, "ERROR: Missing font");

                auto& textView = *(static_cast<TextView*>(&view));
                textView.Configure(TextView::Config{
                    .text = config.text, .fontSpec = config.fontSpec, .color = config.color });
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

// NOTE: Slider isn't dynamic because we need to know the axis of the slider when we create it
// FUTURE: add dynamic slider support if needed
This& ViewBuilder::SliderView(SliderViewConfig config) {
    QB().And(config.name)
        .Slider({ .axis = config.axis,
                  .valueBinding = config.valueBinding,
                  .trackOrthogonal = config.trackOrthogonal })
        .Pop();

    return *this;
}

This& ViewBuilder::ButtonView(BuildConfigFunc<ButtonViewConfig> buildConfigFunc) {
    QB().And("")
        .With<ButtonControl>()
        .ModifyLatest<ButtonControl>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                ViewBuilder vb(*view.owner);
                vb.AddViewAttachments(
                    view, { .name = config.name,
                            .buildBackgroundFunc = config.buildFrameFunc,
                            .buildViewFunc = config.buildLabelFunc }
                );

                auto& button = *(static_cast<ButtonControl*>(&view));
                button.onPressFunc = config.onPressFunc;

                if (config.onControlChangeFunc) {
                    button.SetOnControlChangeFunc(config.onControlChangeFunc);
                }

                auto currentCollider = button.owner->TypeComponent<Collider2D>();
                if (config.makeColliderFunc) {
                    SP<SomeWorldComponent> collider = config.makeColliderFunc();
                    if (collider) {
                        button.owner->RemoveType<Collider2D>();
                        button.owner->Add(collider);
                    }
                } else if (nullptr == currentCollider) {
                    QuickBuilder(*button.owner).RectCollider();
                }

                if (config.modifyViewFunc) {
                    config.modifyViewFunc(view);
                }
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::ToggleButtonView(BuildConfigFunc<ToggleButtonViewConfig> buildConfigFunc) {
    QB().And("")
        .With<ToggleButtonControl>()
        .ModifyLatest<ToggleButtonControl>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                ViewBuilder vb(*view.owner);
                vb.AddViewAttachments(
                    view, { .name = config.name,
                            .buildBackgroundFunc = config.buildFrameFunc,
                            .buildViewFunc = config.buildLabelFunc }
                );

                auto& button = *(static_cast<ToggleButtonControl*>(&view));
                if (config.onControlChangeFunc) {
                    button.SetOnControlChangeFunc(config.onControlChangeFunc);
                }

                auto currentCollider = button.owner->TypeComponent<Collider2D>();
                if (config.makeColliderFunc) {
                    SP<SomeWorldComponent> collider = config.makeColliderFunc();
                    if (collider) {
                        button.owner->RemoveType<Collider2D>();
                        button.owner->Add(collider);
                    }
                } else if (nullptr == currentCollider) {
                    QuickBuilder(*button.owner).RectCollider();
                }

                if (config.isOnBinding.IsValid()) {
                    button.SetIsOnBinding(config.isOnBinding);
                }

                if (config.modifyViewFunc) {
                    config.modifyViewFunc(view);
                }
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::Immediate(BuildConfigFunc<ImmediateConfig> buildConfigFunc) {
    QB().And("").With<View2D>();

    AssociateImmediate(buildConfigFunc);
    QB().Pop();

    return *this;
}

This& ViewBuilder::AssociateImmediate(BuildConfigFunc<ImmediateConfig> buildConfigFunc) {
    QB().ModifyLatest<View2D>([&](auto& view) {
        view.buildViewFunc = [=](auto& view) {
            GUARD(buildConfigFunc)
            GUARD(view.owner)

            auto config = buildConfigFunc(view);
            view.owner->SetName(config.name);

            auto& renderer = view.owner->template AddComponentIfNeeded<ImRenderer>();

            View2D* viewPtr = &view;

            renderer.AddSignalHandler({ .id = SignalId::RenderPrepare,
                                        .func = [=](auto& renderer, auto& signal) {
                                            ImRenderer& imRenderer =
                                                *(static_cast<ImRenderer*>(&renderer));
                                            if (config.renderFunc) {
                                                config.renderFunc(*viewPtr, imRenderer);
                                            }
                                        } });

            if (config.modifyRendererFunc) {
                config.modifyRendererFunc(renderer);
            }
        };

        view.Rebuild();
    });

    return *this;
}

This& ViewBuilder::Pages(BuildConfigFunc<PagesConfig> buildConfigFunc) {
    QB().And("")
        .With<PageView>(nullptr)
        .ModifyLatest<PageView>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                auto pageView = static_cast<PageView*>(&view);

                if (config.store) {
                    pageView->SetStore(config.store);
                }

                pageView->buildPageFuncs.clear();
                pageView->pageOrder.clear();

                for (auto& page : config.pages) {
                    pageView->buildPageFuncs.insert_or_assign(page.id, page.buildPageFunc);
                    pageView->pageOrder.push_back(page.id);
                }

                UP<SomeViewLayout> layout = NEW<ZStackViewLayout>();
                view.SetLayout(layout);

                auto store = pageView->Store();
                if (!IsEmpty(config.selectedPage) && !store->HasSelection()) {
                    pageView->SelectPage(config.selectedPage);
                }
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::View(ViewConfig config) {
    QB().And(config.name).With<View2D>().ModifyLatest<View2D>(config.modifyViewFunc).Pop();

    return *this;
}

This& ViewBuilder::Spacer() {
    QB().And("Spacer").With<SpacerView>().Pop();

    return *this;
}

This& ViewBuilder::FixedSize(std::optional<float> width, std::optional<float> height) {
    QB().ModifyLatest<View2D>([&](auto& view) { view.SetFixedSize(width, height); });

    return *this;
}

This& ViewBuilder::Offset(float x, float y) {
    QB().ModifyLatest<View2D>([&](auto& view) { view.SetOffset(x, y); });

    return *this;
}

This& ViewBuilder::SetIsIdealSize(bool isX, bool isY) {
    QB().ModifyLatest<View2D>([&](auto& view) { view.SetIsIdealSize(isX, isY); });

    return *this;
}

This& ViewBuilder::RootView(Vector2 size, BuildViewFunc buildFunc) {
    QB().With<View2D>().ModifyLatest<View2D>([=](auto& view) {
        view.SetFrameSize(size);

        view.buildViewFunc = [=](auto& view) {
            ViewBuilder viewBuilder(*view.owner);
            buildFunc(viewBuilder);
        };

        view.Rebuild();
        view.LayoutIfNeeded();
    });

    return *this;
}

View2D* ViewBuilder::ActiveView() const {
    return QB().Node().TypeComponent<View2D>();
}

World* ViewBuilder::GetWorld() const {
    return QB().Node().World();
}

DesignSystem* ViewBuilder::GetDesignSystem() const {
    return QB().Node().World()->designSystem.get();
}

This& ViewBuilder::RadioButtonGroup(RadioButtonGroupConfig config) {
    GUARDR_LOG(config.store, *this, "ERROR. Missing store for radio button group")

    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)

    auto spacing = designSystem->theme->ElementSpacing(UIElementId::ControlGroup, { 0, 16 });

    VStack({ .spacing = spacing.y, .alignFunc = AlignFuncs::left, .buildViewFunc = [=](auto& vb) {
                for (int i = 0; i < config.options.size(); i++) {
                    auto& option = config.options[i];
                    String id = MakeString(i);

                    vb.RadioButton({ .label = option,
                                     .isOnBinding = {
                                         [=]() { return config.store->selection.Value() == id; },
                                         [=](auto& value) {
                                             if (value)
                                                 config.store->selection = id;
                                         } } });
                }
            } });

    return *this;
}

This& ViewBuilder::SegmentedPicker(SegmentedPickerConfig config) {
    GUARDR_LOG(config.store, *this, "ERROR. Missing store for segmented picker")

    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)

    HStack({ .buildViewFunc = [=](auto& vb) {
        for (int i = 0; i < config.options.size(); i++) {
            auto& option = config.options[i];
            String id = MakeString(i);

            vb.SegmentToggle({ .label = option,
                               .isOnBinding = {
                                   [=]() { return config.store->selection.Value() == id; },
                                   [=](auto& value) {
                                       if (value)
                                           config.store->selection = id;
                                   } } });
        }
    } });

    return *this;
}

This& ViewBuilder::Surface(SurfaceConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::Surface, config, *this);
    return *this;
}

This& ViewBuilder::Button(ButtonConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::Button, config, *this);
    return *this;
}

This& ViewBuilder::SegmentToggle(ToggleButtonConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::SegmentToggle, config, *this);
    return *this;
}

This& ViewBuilder::ImageToggle(ToggleButtonConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::ImageToggle, config, *this);
    return *this;
}

This& ViewBuilder::SwitchToggle(ToggleButtonConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::SwitchToggle, config, *this);
    return *this;
}

This& ViewBuilder::Dial(DialConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::Dial, config, *this);
    return *this;
}

This& ViewBuilder::Slider(SliderConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::Slider, config, *this);
    return *this;
}

This& ViewBuilder::ProgressBar(ProgressBarConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::ProgressBar, config, *this);
    return *this;
}

This& ViewBuilder::ProgressCircle(ProgressBarConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::ProgressCircle, config, *this);
    return *this;
}

This& ViewBuilder::Label(LabelConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::Label, config, *this);
    return *this;
}

This& ViewBuilder::CheckButton(ToggleButtonConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::CheckButton, config, *this);
    return *this;
}

This& ViewBuilder::RadioButton(ToggleButtonConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::RadioButton, config, *this);
    return *this;
}

This& ViewBuilder::Toast(LabelConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(UIItemId::Toast, config, *this);
    return *this;
}

This& ViewBuilder::AddToolTip(ToolTipConfig config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)

    auto activeView = ActiveView();
    GUARDR(activeView, *this)

    // Add a collider if needed
    if (nullptr == activeView->owner->TypeComponent<Collider2D>()) {
        QuickBuilder(*activeView->owner).RectCollider();
    }

    activeView->AddSignalHandler<HoverUIEvent>(
        { .id = SignalId::Hover,
          .func =
              [=, toolTipView = (View2D*)nullptr](auto& owner, auto& event) mutable {
                  auto& view = *(static_cast<View2D*>(&owner));

                  if (event.isHovering) {
                      GUARD(nullptr == toolTipView)
                      view.Present(
                          { .makeFrameFunc =
                                [=](auto& presentingView, auto& view) {
                                    float width = config.width;
                                    view.SetFixedSize(width, {});

                                    ViewSizeProposal proposal({ .width = width,
                                                                .height = FloatMath::maxValue });
                                    auto proxy = view.MakeViewProxy();
                                    Vector2 frameSize = proxy.ViewSize(proposal);

                                    auto presentingSize = presentingView.Bounds().size;
                                    Vector2 presentingAnchor =
                                        config.presentingAnchor.AnchorPosition(presentingSize);
                                    Vector2 viewAnchor = config.anchor.AnchorPosition(frameSize);

                                    Vector2 rootPresentingAnchor =
                                        presentingView.ToRootViewPosition(presentingAnchor);
                                    Vector2 popoverPosition = rootPresentingAnchor - viewAnchor;

                                    return Rect{ .origin = popoverPosition + config.offset,
                                                 .size = frameSize };
                                },
                            .buildViewFunc =
                                [=, &toolTipView](auto& vb) {
                                    designSystem->BuildView(
                                        UIItemId::ToolTip,
                                        ToolTipConfig{ .text = config.text,
                                                       .result = &toolTipView },
                                        vb
                                    );
                                } }
                      );
                  } else {
                      GUARD(toolTipView)
                      view.Dismiss(*toolTipView);
                      toolTipView = nullptr;
                  }
              } }
    );

    return *this;
}
