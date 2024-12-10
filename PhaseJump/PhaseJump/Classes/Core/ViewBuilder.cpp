#include "ViewBuilder.h"
#include "ColorView.h"
#include "FlowStackViewLayout.h"
#include "Font.h"
#include "ImRenderer.h"
#include "MatchSizeViewLayout.h"
#include "PadViewLayout.h"
#include "QuickBuild.h"
#include "SliderControl.h"
#include "SomeCollider2D.h"
#include "SpacerView.h"
#include "TextRenderer.h"
#include "TextView.h"
#include "ToggleButtonControl.h"
#include "View2D.h"
#include "World.h"
#include "ZStackViewLayout.h"

using namespace std;
using namespace PJ;

using This = ViewBuilder;

ViewBuilder::ViewBuilder(WorldNode& node) {
    qb = NEW<QuickBuild>(node);
}

This& ViewBuilder::Pad(BuildConfigFunc<PadConfig> buildConfigFunc) {
    qb->And()
        .With<View2D>()
        .ModifyLatest<View2D>([=](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

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
    qb->And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

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
    qb->And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

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

This& ViewBuilder::ZStack(BuildConfigFunc<ZStackConfig> buildConfigFunc) {
    qb->And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

                UP<SomeViewLayout> layout =
                    NEW<ZStackViewLayout>(config.xAlignFunc, config.yAlignFunc);
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

This& ViewBuilder::Background(BuildViewFunc buildViewFunc) {
    // TODO: This is a WORK IN PROGRESS. Not ready yet
    qb->ModifyLatest<View2D>([&](auto& activeView) {
        auto parentView = activeView.ParentView();
        GUARD_LOG(parentView, "ERROR: Missing parent view for background")

        ViewBuilder vb(*parentView->owner);
        buildViewFunc(vb);

        vb.QB().ModifyLatest<View2D>([&](auto& view) {
            SP<View2D> activeViewShared = SCAST<View2D>(activeView.shared_from_this());
            UP<SomeViewLayout> layout = NEW<MatchSizeViewLayout>(activeViewShared);
            view.SetLayout(layout);

            // TODO: parentView->owner->Move(view, parentView->owner->ChildIndex(*activeView));
        });
    });

    return *this;
}

This& ViewBuilder::MatchZStack(BuildConfigFunc<MatchZStackConfig> buildConfigFunc) {
    qb->And()
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

                GUARD(config.buildViewFunc)

                ViewBuilder vb(*view.owner);
                vb.AddMatchZStack(view, config);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::AddMatchZStack(View2D& view, MatchZStackConfig config) {
    UP<SomeViewLayout> layout = NEW<ZStackViewLayout>(AlignFuncs::center, AlignFuncs::center);
    view.SetLayout(layout);

    ViewBuilder backgroundViewBuilder(qb->Node());
    ViewBuilder viewBuilder(qb->Node());
    ViewBuilder overlayViewBuilder(qb->Node());

    if (config.buildBackgroundFunc) {
        config.buildBackgroundFunc(backgroundViewBuilder);
    }

    if (config.buildViewFunc) {
        config.buildViewFunc(viewBuilder);
    }

    auto zStackView = qb->Node().TypeComponent<View2D>();
    auto childViews = zStackView->ChildViews();
    auto matchView = childViews.size() > 0 ? childViews[childViews.size() - 1] : nullptr;

    if (config.buildOverlayFunc) {
        config.buildOverlayFunc(overlayViewBuilder);
    }

    if (config.buildViewFunc && matchView) {
        SP<View2D> matchViewShared = SCAST<View2D>(matchView->shared_from_this());

        if (config.buildBackgroundFunc) {
            backgroundViewBuilder.qb->ModifyLatest<View2D>([&](auto& view) {
                UP<SomeViewLayout> layout = NEW<MatchSizeViewLayout>(matchViewShared);
                view.SetLayout(layout);
            });
        }

        if (config.buildOverlayFunc) {
            overlayViewBuilder.qb->ModifyLatest<View2D>([&](auto& view) {
                UP<SomeViewLayout> layout = NEW<MatchSizeViewLayout>(matchViewShared);
                view.SetLayout(layout);
            });
        }
    }

    qb->Pop();

    return *this;
}

This& ViewBuilder::Color(BuildConfigFunc<ColorConfig> buildConfigFunc) {
    qb->And("")
        .With<ColorView>()
        .ModifyLatest<ColorView>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

                static_cast<ColorView*>(&view)->SetColor(config.color);
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::Text(BuildConfigFunc<TextConfig> buildConfigFunc) {
    qb->And("")
        .With<TextView>()
        .ModifyLatest<TextView>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

                auto world = view.owner->World();
                GUARD(world)

                auto font = world->FindFont(config.fontSpec);
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
This& ViewBuilder::Slider(SliderConfig config) {
    qb->And(config.name)
        .Slider({ .axis = config.axis,
                  .valueBinding = config.valueBinding,
                  .trackOrthogonal = config.trackOrthogonal })
        .Pop();

    return *this;
}

This& ViewBuilder::Button(BuildConfigFunc<ButtonConfig> buildConfigFunc) {
    qb->And("")
        .With<ButtonControl>()
        .ModifyLatest<ButtonControl>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

                ViewBuilder vb(*view.owner);
                vb.AddMatchZStack(
                    view, { .name = config.name,
                            .buildBackgroundFunc = config.buildFrameFunc,
                            .buildViewFunc = config.buildLabelFunc }
                );

                auto& button = *(static_cast<ButtonControl*>(&view));
                button.onPressFunc = config.onPressFunc;

                if (config.onControlChangeFunc) {
                    button.SetOnControlChangeFunc(config.onControlChangeFunc);
                }

                auto currentCollider = button.owner->TypeComponent<SomeCollider2D>();
                if (config.makeColliderFunc) {
                    SP<SomeWorldComponent> collider = config.makeColliderFunc();
                    if (collider) {
                        button.owner->RemoveType<SomeCollider2D>();
                        button.owner->Add(collider);
                    }
                } else if (nullptr == currentCollider) {
                    QuickBuild(*button.owner).RectCollider({ 1, 1 });
                }
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::ToggleButton(BuildConfigFunc<ToggleButtonConfig> buildConfigFunc) {
    qb->And("")
        .With<ToggleButtonControl>()
        .ModifyLatest<ToggleButtonControl>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

                ViewBuilder vb(*view.owner);
                vb.AddMatchZStack(
                    view, { .name = config.name,
                            .buildBackgroundFunc = config.buildFrameFunc,
                            .buildViewFunc = config.buildLabelFunc }
                );

                auto& button = *(static_cast<ToggleButtonControl*>(&view));
                if (config.onControlChangeFunc) {
                    button.SetOnControlChangeFunc(config.onControlChangeFunc);
                }

                auto currentCollider = button.owner->TypeComponent<SomeCollider2D>();
                if (config.makeColliderFunc) {
                    SP<SomeWorldComponent> collider = config.makeColliderFunc();
                    if (collider) {
                        button.owner->RemoveType<SomeCollider2D>();
                        button.owner->Add(collider);
                    }
                } else if (nullptr == currentCollider) {
                    QuickBuild(*button.owner).RectCollider({ 1, 1 });
                }

                if (config.isOnBinding.IsValid()) {
                    button.SetIsOnBinding(config.isOnBinding);
                }
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::Immediate(BuildConfigFunc<ImmediateConfig> buildConfigFunc) {
    qb->And("")
        .With<View2D>()
        .ModifyLatest<View2D>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

                auto& renderer = view.owner->template AddComponentIfNeeded<ImRenderer>();

                View2D* viewPtr = &view;

                renderer.signalFuncs[SignalId::RenderPrepare] = [=](auto& renderer, auto& signal) {
                    ImRenderer& imRenderer = *(static_cast<ImRenderer*>(&renderer));
                    if (config.renderFunc) {
                        config.renderFunc(*viewPtr, imRenderer);
                    }
                };

                if (config.modifyRendererFunc) {
                    config.modifyRendererFunc(renderer);
                }
            };

            view.Rebuild();
        })
        .Pop();

    return *this;
}

This& ViewBuilder::Pages(BuildConfigFunc<PagesConfig> buildConfigFunc) {
    qb->And("")
        .With<PageView>(nullptr)
        .ModifyLatest<PageView>([&](auto& view) {
            view.buildViewFunc = [=](auto& view) {
                GUARD(buildConfigFunc)
                GUARD(view.owner)

                auto config = buildConfigFunc(view);
                view.owner->name = config.name;

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
    qb->And(config.name).With<View2D>().ModifyLatest<View2D>(config.modifyViewFunc).Pop();

    return *this;
}

This& ViewBuilder::Spacer() {
    qb->And("Spacer").With<SpacerView>().Pop();

    return *this;
}

This& ViewBuilder::FixedSize(std::optional<float> width, std::optional<float> height) {
    qb->ModifyLatest<View2D>([&](auto& view) { view.SetFixedSize(width, height); });

    return *this;
}

This& ViewBuilder::Offset(float x, float y) {
    qb->ModifyLatest<View2D>([&](auto& view) { view.SetOffset(x, y); });

    return *this;
}

This& ViewBuilder::SetIsIdealSize(bool isX, bool isY) {
    qb->ModifyLatest<View2D>([&](auto& view) { view.SetIsIdealSize(isX, isY); });

    return *this;
}

This& ViewBuilder::RootView(Vector2 size, BuildViewFunc buildFunc) {
    qb->With<View2D>().ModifyLatest<View2D>([=](auto& view) {
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
    return qb->Node().TypeComponent<View2D>();
}

World* ViewBuilder::GetWorld() const {
    return qb->Node().World();
}

DesignSystem* ViewBuilder::GetDesignSystem() const {
    return qb->Node().World()->designSystem.get();
}
