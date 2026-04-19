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

ViewBuilder::Scope::~Scope() {
    Leave();
}

ViewBuilder::Scope::Scope(Scope&& other) noexcept :
    vb(other.vb),
    componentsBaseSize(other.componentsBaseSize) {
    other.vb = nullptr;
}

ViewBuilder::Scope& ViewBuilder::Scope::operator=(Scope&& other) noexcept {
    if (this != &other) {
        Leave();
        vb = other.vb;
        componentsBaseSize = other.componentsBaseSize;
        other.vb = nullptr;
    }
    return *this;
}

void ViewBuilder::Scope::Leave() {
    if (vb) {
        // Isolate child components from the parent's ModifyLatest target: truncate the list
        // back to the point just after the scope's own view was added. Children remain as
        // actual child world nodes; only the component-list cursor is reset.
        vb->QB().TruncateComponentsToSize(componentsBaseSize);
        vb->QB().Pop();
        vb = nullptr;
    }
}

template <class Config>
ViewBuilder::Scope ViewBuilder::PushLayoutView(
    Config config, std::function<UP<SomeViewLayout>(Config const&)> makeLayoutFunc
) {
    QB().And("").With<View2D>();

    auto view = QB().Node().TypeComponent<View2D>();
    if (view) {
        view->owner->SetName(config.name);
        UP<SomeViewLayout> layout = makeLayoutFunc(config);
        view->SetLayout(layout);
    }

    return Scope(*this, QB().ComponentsSize());
}

ViewBuilder::Scope ViewBuilder::BeginVStack(VStackConfig config) {
    return PushLayoutView<VStackConfig>(config, [](VStackConfig const& c) {
        return NEW<FlowStackViewLayout>(Axis2D::Y, c.spacing, c.alignFunc);
    });
}

ViewBuilder::Scope ViewBuilder::BeginHStack(HStackConfig config) {
    return PushLayoutView<HStackConfig>(config, [](HStackConfig const& c) {
        return NEW<FlowStackViewLayout>(Axis2D::X, c.spacing, c.alignFunc);
    });
}

ViewBuilder::Scope ViewBuilder::BeginZStack(ZStackConfig config) {
    return PushLayoutView<ZStackConfig>(config, [](ZStackConfig const& c) {
        return NEW<ZStackViewLayout>(c.alignment.xAlignFunc, c.alignment.yAlignFunc);
    });
}

ViewBuilder::Scope ViewBuilder::BeginPad(PadConfig config) {
    return PushLayoutView<PadConfig>(config, [](PadConfig const& c) {
        return NEW<PadViewLayout>(c.insets);
    });
}

ViewBuilder::Scope ViewBuilder::BeginFixedGrid(FixedGridConfig config) {
    return PushLayoutView<FixedGridConfig>(config, [](FixedGridConfig const& c) {
        return NEW<FixedGridViewLayout>(FixedGridViewLayout::Config{ .gridSize = c.gridSize,
                                                                     .cellSize = c.cellSize,
                                                                     .alignment = c.alignment,
                                                                     .spacing = c.spacing });
    });
}

This& ViewBuilder::Rebuildable(BuildViewFunc buildFunc) {
    auto view = QB().Node().TypeComponent<View2D>();
    GUARDR_LOG(view, *this, "ERROR: Rebuildable needs an active View2D")

    view->buildViewFunc = [buildFunc](View2D& view) {
        GUARD(view.owner)
        if (buildFunc) {
            ViewBuilder vb(*view.owner);
            buildFunc(vb);
        }
    };

    // Build initial children. Don't use view->Rebuild() — it unconditionally destroys existing
    // children, which would clobber anything added imperatively before this call.
    if (buildFunc) {
        ViewBuilder vb(*view->owner);
        buildFunc(vb);
    }

    return *this;
}

template <class Config>
This& ViewBuilder::AddLayoutView(
    BuildConfigFunc<Config> buildConfigFunc,
    std::function<UP<SomeViewLayout>(Config const&)> makeLayoutFunc
) {
    QB().And()
        .With<View2D>()
        .ModifyLatest<View2D>([=](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->SetName(config.name);

                UP<SomeViewLayout> layout = makeLayoutFunc(config);
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

This& ViewBuilder::AddAttachment(BuildViewFunc buildFunc, bool isBackground) {
    QB().ModifyLatest<View2D>([=](auto& activeView) {
        auto parentView = activeView.ParentView();
        GUARD_LOG(
            parentView, isBackground ? "ERROR: Missing parent view for background"
                                     : "ERROR: Missing parent view for overlay"
        )

        auto viewNode = parentView->owner->Move(*activeView.owner);

        ViewBuilder vb(*parentView->owner);
        vb.ViewAttachments([=](auto& view) {
            ViewAttachmentsConfig config{ .viewNode = viewNode };
            if (isBackground) {
                config.buildBackgroundFunc = buildFunc;
            } else {
                config.buildOverlayFunc = buildFunc;
            }
            return config;
        });
    });

    return *this;
}

This& ViewBuilder::Pad(BuildConfigFunc<PadConfig> buildConfigFunc) {
    return AddLayoutView<PadConfig>(buildConfigFunc, [](PadConfig const& config) {
        return NEW<PadViewLayout>(config.insets);
    });
}

This& ViewBuilder::HStack(BuildConfigFunc<HStackConfig> buildConfigFunc) {
    return AddLayoutView<HStackConfig>(buildConfigFunc, [](HStackConfig const& config) {
        return NEW<FlowStackViewLayout>(Axis2D::X, config.spacing, config.alignFunc);
    });
}

This& ViewBuilder::VStack(BuildConfigFunc<VStackConfig> buildConfigFunc) {
    return AddLayoutView<VStackConfig>(buildConfigFunc, [](VStackConfig const& config) {
        return NEW<FlowStackViewLayout>(Axis2D::Y, config.spacing, config.alignFunc);
    });
}

This& ViewBuilder::FixedGrid(BuildConfigFunc<FixedGridConfig> buildConfigFunc) {
    return AddLayoutView<FixedGridConfig>(buildConfigFunc, [](FixedGridConfig const& config) {
        return NEW<FixedGridViewLayout>(FixedGridViewLayout::Config{ .gridSize = config.gridSize,
                                                                     .cellSize = config.cellSize,
                                                                     .alignment = config.alignment,
                                                                     .spacing = config.spacing });
    });
}

This& ViewBuilder::ZStack(BuildConfigFunc<ZStackConfig> buildConfigFunc) {
    return AddLayoutView<ZStackConfig>(buildConfigFunc, [](ZStackConfig const& config) {
        return NEW<ZStackViewLayout>(config.alignment.xAlignFunc, config.alignment.yAlignFunc);
    });
}

This& ViewBuilder::Background(BuildViewFunc buildBackgroundFunc) {
    return AddAttachment(buildBackgroundFunc, true);
}

This& ViewBuilder::Overlay(BuildViewFunc buildOverlayFunc) {
    // Important: Add Background first or the view order will be wrong
    // FUTURE: make this more intelligent if needed
    return AddAttachment(buildOverlayFunc, false);
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

template <class Control, class Config>
This& ViewBuilder::AddControlView(
    BuildConfigFunc<Config> buildConfigFunc,
    std::function<void(Control&, Config const&)> configureControlFunc
) {
    QB().And("")
        .With<Control>()
        .template ModifyLatest<Control>([=](auto& view) {
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

                auto& control = *(static_cast<Control*>(&view));

                if (config.onViewStateChangeFunc) {
                    control.SetOnViewStateChangeFunc(config.onViewStateChangeFunc);
                }

                auto currentCollider = control.owner->template TypeComponent<Collider2D>();
                if (config.makeColliderFunc) {
                    SP<SomeWorldComponent> collider = config.makeColliderFunc();
                    if (collider) {
                        control.owner->template RemoveType<Collider2D>();
                        control.owner->Add(collider);
                    }
                } else if (nullptr == currentCollider) {
                    QuickBuilder(*control.owner).RectCollider();
                }

                if (configureControlFunc) {
                    configureControlFunc(control, config);
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

This& ViewBuilder::ButtonView(BuildConfigFunc<ButtonViewConfig> buildConfigFunc) {
    return AddControlView<ButtonControl, ButtonViewConfig>(
        buildConfigFunc, [](ButtonControl& control, ButtonViewConfig const& config
                         ) { control.onPressFunc = config.onPressFunc; }
    );
}

This& ViewBuilder::ToggleButtonView(BuildConfigFunc<ToggleButtonViewConfig> buildConfigFunc) {
    return AddControlView<ToggleButtonControl, ToggleButtonViewConfig>(
        buildConfigFunc,
        [](ToggleButtonControl& control, ToggleButtonViewConfig const& config) {
            if (config.isOnBinding.IsValid()) {
                control.SetIsOnBinding(config.isOnBinding);
            }
        }
    );
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
    auto world = GetWorld();
    GUARDR(world, {})
    return world->designSystem.get();
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

                    vb.RadioButton(
                        { .label = option,
                          .isOnBinding = Binding<bool>(
                              { .getFunc = [=]() { return config.store->selection.Value() == id; },
                                .setFunc =
                                    [=](auto& value) {
                                        if (value)
                                            config.store->selection = id;
                                    } }
                          ) }
                    );
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

            vb.SegmentToggle(
                { .label = option,
                  .isOnBinding = Binding<bool>(
                      { .getFunc = [=]() { return config.store->selection.Value() == id; },
                        .setFunc =
                            [=](auto& value) {
                                if (value)
                                    config.store->selection = id;
                            } }
                  ) }
            );
        }
    } });

    return *this;
}

template <class Config>
This& ViewBuilder::BuildFromDesignSystem(String elementId, Config config) {
    auto designSystem = GetDesignSystem();
    GUARDR(designSystem, *this)
    designSystem->BuildView(elementId, config, *this);
    return *this;
}

This& ViewBuilder::Surface(SurfaceConfig config) {
    return BuildFromDesignSystem(UIItemId::Surface, config);
}

This& ViewBuilder::Button(ButtonConfig config) {
    return BuildFromDesignSystem(UIItemId::Button, config);
}

This& ViewBuilder::SegmentToggle(ToggleButtonConfig config) {
    return BuildFromDesignSystem(UIItemId::SegmentToggle, config);
}

This& ViewBuilder::ImageToggle(ToggleButtonConfig config) {
    return BuildFromDesignSystem(UIItemId::ImageToggle, config);
}

This& ViewBuilder::SwitchToggle(ToggleButtonConfig config) {
    return BuildFromDesignSystem(UIItemId::SwitchToggle, config);
}

This& ViewBuilder::Dial(DialConfig config) {
    return BuildFromDesignSystem(UIItemId::Dial, config);
}

This& ViewBuilder::Slider(SliderConfig config) {
    return BuildFromDesignSystem(UIItemId::Slider, config);
}

This& ViewBuilder::ProgressBar(ProgressBarConfig config) {
    return BuildFromDesignSystem(UIItemId::ProgressBar, config);
}

This& ViewBuilder::ProgressCircle(ProgressBarConfig config) {
    return BuildFromDesignSystem(UIItemId::ProgressCircle, config);
}

This& ViewBuilder::Label(LabelConfig config) {
    return BuildFromDesignSystem(UIItemId::Label, config);
}

This& ViewBuilder::CheckButton(ToggleButtonConfig config) {
    return BuildFromDesignSystem(UIItemId::CheckButton, config);
}

This& ViewBuilder::RadioButton(ToggleButtonConfig config) {
    return BuildFromDesignSystem(UIItemId::RadioButton, config);
}

This& ViewBuilder::Toast(LabelConfig config) {
    return BuildFromDesignSystem(UIItemId::Toast, config);
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
