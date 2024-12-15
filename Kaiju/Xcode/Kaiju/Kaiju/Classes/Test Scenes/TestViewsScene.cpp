#include "TestViewsScene.h"

using namespace std;
using namespace PJ;

TestViewsScene::TestViewsScene() {
    layouts = { { .id = "1080", .size = { 1920, 1080 } },
                { .id = "phone", .size = { 750, 1334 } } };

    PlanUIFunc planUIFunc = [this](auto& component, String context, UIPlanner& planner) {
        // TODO: should this be optional<int>? for no selection?
        Binding<int> binding{ [this]() { return LayoutSizeIndex() ? *LayoutSizeIndex() : -1; },
                              [this](auto& value) { SetLayoutSizeIndex(value); } };
        planner.ListSelect("Sizes", { "1080", "phone" }, binding);

        planner.PickerColor(
            "Environment Color",
            { [this]() {
                 auto rootView = RootView();
                 return rootView ? rootView->EnvironmentValue<Color>("color", Color::gray)
                                 : Color::gray;
             },
              [this](auto& value) {
                  auto rootView = RootView();
                  if (rootView) {
                      rootView->SetEnvironmentValue("color", value);
                  }
              } }
        );
    };
    Override(planUIFuncs[UIContextId::Editor], planUIFunc);
}

std::optional<int> TestViewsScene::LayoutSizeIndex() const {
    auto rootView = RootView();
    GUARDR(rootView, {})

    auto rootViewSize = rootView->Frame().size;
    return IndexOfIf(layouts, [&, this](auto& item) { return item.size == rootViewSize; });
}

void TestViewsScene::SetLayoutSizeIndex(int value) {
    GUARD(value >= 0 && value < layouts.size())

    SetLayoutSize(layouts[value].size);
}

View2D* TestViewsScene::RootView() const {
    GUARDR(rootComponent, {})

    GUARDR(rootComponent->owner->Children().size() > 0, {})
    auto child = rootComponent->owner->Children()[0];
    GUARDR(child, {})

    auto view = child->TypeComponent<View2D>();
    GUARDR(view, {})

    return view;
}

void TestViewsScene::SetLayoutSize(Vector2 size) {
    auto view = RootView();
    GUARD(view)

    view->SetFrameSize(size);
}

void TestViewsScene::LoadInto(WorldNode& root) {
    root.name = "TestViewsScene";

    Vector2 _1080{ 1920, 1080 };

    QB(root).OrthoStandard();

    auto& ac = root.With<WorldComponent<>>();
    rootComponent = &ac;
    ac.signalFuncs[SignalId::KeyDown] = [this, &root](auto& component, auto& signal) {
        auto keyDownEvent = dynamic_cast<KeyDownUIEvent const*>(&signal);
        GUARD(keyDownEvent)

        switch (keyDownEvent->keyCode.value) {
        case SDLK_LEFT:
            {
                targetView--;
                if (targetView < 0) {
                    targetView = (int)buildViewModels.size() - 1;
                }

                RebuildTargetView();
                break;
            }
        case SDLK_RIGHT:
            {
                targetView++;
                if (targetView >= buildViewModels.size()) {
                    targetView = 0;
                }

                RebuildTargetView();
                break;
            }
        }
    };

    auto textView = [=](ViewBuilder& vb, String text) -> ViewBuilder& {
        vb.Text({ .fontSpec = { .size = 32 }, .text = text, .color = Color::black })
            .SetIsIdealSize();

        return vb;
    };

    auto colorButton = [=](ViewBuilder& vb, String label,
                           std::function<void(ButtonControl&)> onPressFunc) -> ViewBuilder& {
        vb.Button({ .label = label, .onPressFunc = onPressFunc });
        return vb;
    };

    auto toggleButton = [=](ViewBuilder& vb, String label) -> ViewBuilder& {
        vb.SegmentToggle({ .label = label });
        return vb;
    };

    auto toggleGroupStore = &this->toggleGroupStore;

    auto toggleButtonGroup = [=](ViewBuilder& vb, String id, String label) -> ViewBuilder& {
        vb.ToggleButtonView(
            { .isOnBinding = { [=]() { return toggleGroupStore->selection.Value() == id; },
                               [=](auto& value) {
                                   if (value)
                                       toggleGroupStore->selection = id;
                               } },
              .onControlChangeFunc =
                  [](auto& control) {
                      auto& button = *(static_cast<ToggleButtonControl*>(&control));
                      GUARD(!IsEmpty(button.owner->Children()))

                      auto renderer = button.owner->Children()[0]->TypeComponent<SomeRenderer>();
                      GUARD(renderer)

                      renderer->SetColor(
                          button.IsPressed()    ? Color::yellow
                          : button.IsToggleOn() ? Color::green
                                                : Color::gray
                      );
                  },
              .buildFrameFunc = [](auto& vb) { vb.Color(Color::gray); },
              .buildLabelFunc =
                  [=](auto& vb) {
                      vb.Pad({ .insets = LayoutInsets::Uniform(10),
                               .buildViewFunc = [=](auto& vb) { textView(vb, label); } });
                  } }
        );

        return vb;
    };

#if FALSE
    //    vb.Button({.onControlChangeFunc = [](auto& control) {
    //        control.owner.
    //    }})

    // Has a make collider func
    vb.ToggleButton(
          { .modifyViewFunc = [](auto& view) { view.AddComponent(MAKE<ColorRenderer>()); },
            .onControlChangeFunc =
                [](auto& control) {
                    auto renderer = control.owner->TypeComponent<SomeRenderer>();

                    if (control.IsPressed()) {
                        renderer->SetColor(Color::pink);
                    } else {
                        if (control.IsToggleOn()) {
                            renderer->SetColor(Color::red);
                        } else {
                            renderer->SetColor(Color::green);
                        }
                    }
                } }
    ).FixedSize(30, 30);

//    vb.ToggleButtonGroup({}, [=](auto& vb) {
//        toggleButton();
//        toggleButton();
//        toggleButton();
//    });
#endif

    DuckDesignSystem duck;

#if FALSE
    buildViewModels.push_back({ "Duck", [=](WorldNode& root) {
        QB(root).And("Root View").RootView({.worldSize = { 800, 800 }, .designSystem = duck, .buildViewFunc = [=](auto& vb) {
                    vb.HStack({ .buildViewFunc = [=](auto& vb) {
                        vb.ThemeButton({ .label = "Duck UI" })
                            .ThemeProgressBar({ .color = Color::red,
                                                .valueFunc = []() { return 0.3f; } });
                    } });
                                   });
                               }});
#endif
        auto dialStack = [=, this](ViewBuilder& vb) {
            vb.VStack({ .spacing = 20, .buildViewFunc = [=, this](auto& vb) {
                           vb.HStack({ .buildViewFunc = [=, this](auto& vb) {
                               vb.Spacer().Dial({ .valueBinding = { [this]() { return dialValue; },
                                                                    [this](auto& value) {
                                                                        dialValue = value;
                                                                    } } });
                           } });
                       } });
        };

        auto duckUIStack = [=, this](ViewBuilder& vb) {
            vb.VStack({ .spacing = 20, .buildViewFunc = [=, this](auto& vb) {
                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Label + Color" })
                                   .Label({ .color = Color::red, .text = "Red Label" });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Button" }).Button({ .label = "Duck UI" });
                           } });

                           vb.HStack({ .spacing = 15, .buildViewFunc = [=, this](auto& vb) {
                                          vb.Label({ .text = "Dial" })
                                              .Dial({ .valueBinding = {
                                                          [this]() { return dialValue; },
                                                          [this](auto& value) {
                                                              dialValue = value;
                                                          } } })
                                              .Dial({ .surfaceColor = Color32(106, 162, 137),
                                                      .valueBinding = {
                                                          [this]() { return dialValue2; },
                                                          [this](auto& value) {
                                                              dialValue2 = value;
                                                          } } });
                                      } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Progress Bar" })
                                   .ProgressBar({ .valueFunc = []() { return 0.3f; } });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Progress Bar + Color" })
                                   .ProgressBar({ .color = Color::red,
                                                  .valueFunc = []() { return 0.3f; } });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Progress Circle" })
                                   .ProgressCircle({ .valueFunc = []() { return 0.65f; } });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Segment Toggle" })
                                   .SegmentToggle({ .label = "Toggle" });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Check Button" })
                                   .CheckButton({ .label = "Checked" });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Radio Button Group" })
                                   .RadioButtonGroup({ .store = toggleGroupStore,
                                                       .options = { "Option 1", "Option 2" } });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Segmented Picker" })
                                   .SegmentedPicker({ .store = toggleGroupStore,
                                                      .options = { "First", "Second", "Final" } });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Image Toggle" })
                                   .ImageToggle({ .imageId = "heart-full",
                                                  .size = Vector2{ 100, 100 } });
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Switch Toggle" }).SwitchToggle({});
                           } });

                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                               vb.Label({ .text = "Toast" })
                                   .Toast({ .text = "This is an important announcement" });
                           } });
                       } });
        };

        buildViewModels
            .push_back({ "Duck", [=](WorldNode& root) {
                            QB(root).And("Root View").RootView({ 800, 1800 }, [=](auto& vb) {
                                vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                                    vb.Surface();
                                    vb.Pad({ .insets = LayoutInsets::Uniform(20),
                                             .buildViewFunc = duckUIStack });
                                } });
                            });
                        } });

        buildViewModels.push_back(
            { "Immediate",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView({ 800, 800 }, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color::white)
                              .Immediate({ .renderFunc = [](auto& view, auto& renderer) {
                                  renderer.SetStrokeWidth(4)
                                      .SetStartPathCap(PathCap::Flat)
                                      .SetEndPathCap(PathCap::Round)
                                      .SetPathCorner(PathCorner::Round)
                                      .FillCircle({}, 100, Color::yellow)
                                      .FillCircle({}, 50, Color::green)
                                      .FillCircle({}, 20, Color::blue)
                                      .FrameCircle({}, 20, Color::red)
                                      .FramePolygon({ { 0, 0 }, { 45, 45 }, { 45, 100 } });
                              } });
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Sliders-bug",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color32(33, 60, 89))
                              .Pad({ .insets = LayoutInsets::Uniform(10),
                                     .buildViewFunc = [=](auto& vb) {
                                         vb.HStack({ .buildViewFunc = [=](auto& vb) {
                                             vb.HStack(
                                                   { .alignFunc = AlignFuncs::left,
                                                     .spacing = 10,
                                                     .buildViewFunc =
                                                         [=](auto& vb) {
                                                             for (int i = 0; i < 1; i++) {
                                                                 vb.SliderView(
                                                                     ViewBuilder::SliderViewConfig{}
                                                                 );
                                                             }
                                                         } }
                                             )
                                                 .HStack({ .spacing = 10,
                                                           .buildViewFunc = [=](auto& vb) {
                                                               for (int i = 0; i < 1; i++) {
                                                                   vb.SliderView({ .axis =
                                                                                       Axis2D::Y });
                                                               }
                                                           } });
                                         } });
                                     } });
                      } });
                  });
              } }
        );

        auto strayHStack = [=](ViewBuilder& vb) -> ViewBuilder& {
            vb.HStack({ .spacing = 0, .buildViewFunc = [](auto& vb) {
                           vb.Spacer()
                               .VStack(
                                   { .spacing = 40,
                                     .buildViewFunc =
                                         [](auto& vb) {
                                             vb.Text({ .fontSpec = { .size = 32 },
                                                       .text = "Quit to Main Menu?",
                                                       .color = Color::black })
                                                 .SetIsIdealSize()
                                                 .HStack({ .spacing = 100,
                                                           .buildViewFunc = [](auto& vb) {
                                                               vb.Text({ .fontSpec = { .size = 32 },
                                                                         .text = "NO",
                                                                         .color = Color::white })
                                                                   .SetIsIdealSize()
                                                                   .Text({ .fontSpec = { .size =
                                                                                             32 },
                                                                           .text = "YES",
                                                                           .color = Color::white })
                                                                   .SetIsIdealSize();
                                                           } });
                                         } }
                               )
                               .Spacer();
                       } });

            return vb;
        };

        buildViewModels.push_back(
            { "Stray-v2",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.VStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Spacer()
                              .ZStack({ .buildViewFunc =
                                            [=](auto& vb) {
                                                vb.Pad({ .insets = LayoutInsets::Uniform(50),
                                                         .buildViewFunc = [=](auto& vb
                                                                          ) { strayHStack(vb); } }
                                                ).Background([=](auto& vb) {
                                                    vb.Color(Color(1, 137.0f / 255.0f, 0, 0.9f));
                                                });
                                                ;
                                            } })
                              .Spacer();
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Toggle Button Group",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color32(33, 60, 89))
                              .Pad({ .insets = LayoutInsets::Uniform(10),
                                     .buildViewFunc = [=](auto& vb) {
                                         vb.HStack({ .spacing = 20, .buildViewFunc = [=](auto& vb) {
                                                        toggleButtonGroup(vb, "1", "Toggle");
                                                        toggleButtonGroup(vb, "2", "Toggle");
                                                        toggleButtonGroup(vb, "3", "Toggle");
                                                    } });
                                     } });
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Spinner",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color([](auto& view) {
                                return ViewBuilder::ColorConfig{
                                    .color = view.template EnvironmentValue<Color>("color")
                                };
                            })
                              .Pad({ .insets = LayoutInsets::Uniform(10),
                                     .buildViewFunc = [=](auto& vb) mutable {
                                         vb.HStack({ .spacing = 20,
                                                     .buildViewFunc = [=,
                                                                       spinnerValue =
                                                                           10](auto& vb) mutable {
                                                         colorButton(vb, "L", [&](auto& button) {
                                                             spinnerValue--;
                                                             button.owner->Parent()
                                                                 ->template TypeComponent<View2D>()
                                                                 ->SetNeedsRebuild();
                                                         });
                                                         textView(vb, MakeString(spinnerValue))
                                                             .FixedSize(300, {});
                                                         colorButton(vb, "R", [&](auto& button) {
                                                             spinnerValue++;
                                                             button.owner->Parent()
                                                                 ->template TypeComponent<View2D>()
                                                                 ->SetNeedsRebuild();
                                                         });
                                                     } });
                                     } });
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Toggle Buttons",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color32(33, 60, 89))
                              .Pad({ .insets = LayoutInsets::Uniform(10),
                                     .buildViewFunc = [=](auto& vb) {
                                         vb.HStack([=](View2D& view) {
                                             return ViewBuilder::HStackConfig{
                                                 .spacing =
                                                     view.EnvironmentValue<Color>("color").r * 255,
                                                 .buildViewFunc =
                                                     [=](auto& vb) {
                                                         toggleButton(vb, "Toggle");
                                                         toggleButton(vb, "Toggle");

                                                         if (vb.ActiveView()
                                                                 ->template EnvironmentValue<Color>(
                                                                     "color"
                                                                 )
                                                                 .r > 0.5) {
                                                             toggleButton(vb, "Extra");
                                                         }
                                                     }
                                             };
                                         });
                                     } });
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Buttons",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color32(33, 60, 89))
                              .Pad({ .insets = LayoutInsets::Uniform(10),
                                     .buildViewFunc = [=](auto& vb) {
                                         vb.HStack({ .spacing = 20, .buildViewFunc = [=](auto& vb) {
                                                        colorButton(vb, "Button", {});
                                                        colorButton(vb, "Button", {});
                                                        colorButton(vb, "Button", {});
                                                    } });
                                     } });
                      } });
                  });
              } }
        );

        static SP<SingleSelectStore> pageViewStore = MAKE<SingleSelectStore>();

        buildViewModels.push_back(
            { "Pages",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color32(33, 60, 89)).VStack({ .buildViewFunc = [=](auto& vb) {
                              vb.Pad({ .insets = LayoutInsets::Uniform(10),
                                       .buildViewFunc =
                                           [=](auto& vb) {
                                               vb.HStack({ .spacing = 20,
                                                           .buildViewFunc = [=](auto& vb) {
                                                               colorButton(
                                                                   vb, "Page 1",
                                                                   [](auto& button) {
                                                                       pageViewStore->selection =
                                                                           "1";
                                                                   }
                                                               );
                                                               colorButton(
                                                                   vb, "Page 2",
                                                                   [](auto& button) {
                                                                       pageViewStore->selection =
                                                                           "2";
                                                                   }
                                                               );
                                                           } });
                                           } })
                                  .Pages({ .store = pageViewStore,
                                           .selectedPage = "2",
                                           .pages = { { .id = "1",
                                                        .buildPageFunc =
                                                            [=](auto& node) {
                                                                ViewBuilder vb(node);
                                                                textView(vb, "1");
                                                            } },
                                                      { .id = "2",
                                                        .buildPageFunc =
                                                            [=](auto& node) {
                                                                ViewBuilder vb(node);
                                                                textView(vb, "2");
                                                            } } } })
                                  .QB()
                                  .template ModifyLatest<PageView>([](auto& view) {
                                      view.signalFuncs[SignalId::KeyDown] = [&](auto& component,
                                                                                auto& signal) {
                                          auto keyDownEvent =
                                              dynamic_cast<KeyDownUIEvent const*>(&signal);
                                          GUARD(keyDownEvent)

                                          switch (keyDownEvent->keyCode.value) {
                                          case '8':
                                              view.Navigate(
                                                  PageView::NavigateDirection::Back, true
                                              );
                                              break;
                                          case '9':
                                              view.Navigate(
                                                  PageView::NavigateDirection::Forward, true
                                              );
                                              break;
                                          }
                                      };
                                  });
                          } });
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Sliders",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color32(33, 60, 89))
                              .Pad({ .insets = LayoutInsets::Uniform(10),
                                     .buildViewFunc = [=](auto& vb) {
                                         vb.HStack({ .buildViewFunc = [=](auto& vb) {
                                             vb.VStack(
                                                   { .alignFunc = AlignFuncs::left,
                                                     .spacing = 10,
                                                     .buildViewFunc =
                                                         [=](auto& vb) {
                                                             for (int i = 0; i < 4; i++) {
                                                                 vb.SliderView(
                                                                     ViewBuilder::SliderViewConfig{}
                                                                 );
                                                             }
                                                         } }
                                             )
                                                 .HStack({ .spacing = 10,
                                                           .buildViewFunc = [=](auto& vb) {
                                                               for (int i = 0; i < 4; i++) {
                                                                   vb.SliderView({ .axis =
                                                                                       Axis2D::Y });
                                                               }
                                                           } });
                                         } });
                                     } });
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Alex Kidd",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color::yellow)
                              .Pad({ .insets = LayoutInsets::Uniform(20),
                                     .buildViewFunc = [=](auto& vb) {
                                         vb.HStack({ .buildViewFunc = [=](auto& vb) {
                                             vb.Spacer().VStack(
                                                 { .spacing = 30,
                                                   .buildViewFunc =
                                                       [=](auto& vb) {
                                                           textView(vb, "LOAD GAME");
                                                           textView(vb, "NEW GAME");

                                                           vb.VStack({ .spacing = 5,
                                                                       .buildViewFunc =
                                                                           [=](auto& vb) {
                                                                               textView(
                                                                                   vb,
                                                                                   "CLASSIC MODE"
                                                                               );
                                                                               textView(
                                                                                   vb, "BOSS RUSH"
                                                                               );
                                                                           } })
                                                               .VStack(
                                                                   { .spacing = 5,
                                                                     .buildViewFunc =
                                                                         [=](auto& vb) {
                                                                             textView(
                                                                                 vb, "OPTIONS"
                                                                             );
                                                                             textView(
                                                                                 vb, "CREDITS"
                                                                             );
                                                                             textView(vb, "EXIT");
                                                                         } }
                                                               );
                                                       } }
                                             );
                                         } });
                                     } });
                      } });
                  });
              } }
        );

        auto anthemButton = [=](ViewBuilder& vb, String text, int index) -> ViewBuilder& {
            vb.HStack({ .spacing = 10,
                        .buildViewFunc =
                            [=](auto& vb) {
                                vb.Color(Color32(242, 241, 211))
                                    .FixedSize(20, 20)
                                    .Text({ .fontSpec = { .size = 32 },
                                            .text = text,
                                            .color = Color32(242, 241, 211) })
                                    .SetIsIdealSize();
                            } }
            ).Offset(index * 10, 0);

            return vb;
        };

        buildViewModels.push_back(
            { "Anthem",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [=](auto& vb) {
                          vb.Color(Color32(33, 60, 89))
                              .Pad({ .insets = LayoutInsets::Uniform(10),
                                     .buildViewFunc = [=](auto& vb) {
                                         vb.HStack({ .buildViewFunc = [=](auto& vb) {
                                             vb.VStack(
                                                   { .alignFunc = AlignFuncs::left,
                                                     .spacing = 10,
                                                     .buildViewFunc =
                                                         [=](auto& vb) {
                                                             VectorList<String> buttons{
                                                                 "GAMEPLAY",  "CONTROLS",
                                                                 "AUDIO",     "VIDEO",
                                                                 "INTERFACE", "ACCESSIBILITY",
                                                                 "ACCOUNT",   "OTHER",
                                                                 "QUIT GAME"
                                                             };

                                                             for (int i = 0; i < buttons.size();
                                                                  i++) {
                                                                 anthemButton(vb, buttons[i], i);
                                                             }
                                                         } }
                                             ).Spacer();
                                         } });
                                     } });
                      } });
                  });
              } }
        );

        buildViewModels.push_back(
            { "Stray- no VStack",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.VStack({ .buildViewFunc = [](auto& vb) {
                          vb.Spacer()
                              .ZStack(
                                  { .buildViewFunc =
                                        [](auto& vb) {
                                            vb.Color(Color(1, 137.0f / 255.0f, 0, 1))
                                                .Pad({ .insets = LayoutInsets::Uniform(50),
                                                       .buildViewFunc = [](auto& vb) {
                                                           vb.HStack(
                                                               { .spacing = 100,
                                                                 .buildViewFunc =
                                                                     [](auto& vb) {
                                                                         vb.Text(
                                                                               { .fontSpec = { .size =
                                                                                                   32 },
                                                                                 .text = "YES",
                                                                                 .color =
                                                                                     Color::white }
                                                                         )
                                                                             .SetIsIdealSize();
                                                                     } }
                                                           );
                                                       } });
                                        } }
                              )
                              .Spacer();
                      } });
                  });
              } }
        );

        String lorem =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas venenatis iaculis "
            "diam, "
            "ac efficitur ex sollicitudin et. Donec lacinia et ex a egestas. Integer sagittis, "
            "neque "
            "non posuere fringilla, magna augue sagittis dolor, et interdum quam nisi a massa. Sed "
            "eu "
            "euismod massa, non posuere nisl. Nunc eu velit ut velit bibendum convallis. Aliquam "
            "condimentum eu ligula ut sollicitudin. Proin tristique fringilla ex vitae egestas. "
            "Duis "
            "sapien erat, suscipit eget facilisis et, lobortis non mi. Donec euismod massa vitae "
            "mi "
            "lacinia fringilla. Nullam et viverra erat. Nam facilisis feugiat molestie. Cras "
            "rutrum "
            "porttitor felis at tristique.";

        buildViewModels.push_back({ "Text Stack", [=](WorldNode& root) {
                                       QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                                           vb.HStack({ .buildViewFunc = [=](auto& vb) {
                                               vb.Text({ .fontSpec = { .size = 32 },
                                                         .text = lorem,
                                                         .color = Color::black })
                                                   .Text({ .fontSpec = { .size = 32 },
                                                           .text = lorem,
                                                           .color = Color::black });
                                           } });
                                       });
                                   } });

        buildViewModels.push_back(
            { "Stray",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                      vb.VStack({ .buildViewFunc = [](auto& vb) {
                          vb.Spacer()
                              .MatchZStack({ .buildBackgroundFunc =
                                                 [](auto& vb) {
                                                     vb.Color(Color(1, 137.0f / 255.0f, 0, 1));
                                                 },
                                             .buildViewFunc =
                                                 [](auto& vb) {
                                                     vb.Pad({ .insets = LayoutInsets::Uniform(50),
                                                              .buildViewFunc = [](auto& vb) {
                                                                  vb.HStack({ .spacing = 0,
                                                                              .buildViewFunc =
                                                                                  [](auto& vb) {
                                                                                      vb.Spacer()
                                                                                          .VStack({ .spacing =
                                                                                                        40,
                                                                                                    .buildViewFunc =
                                                                                                        [](auto&
                                                                                                               vb
                                                                                                        ) {
                                                                                                            vb.Text(
                                                                                                                  { .fontSpec = { .size =
                                                                                                                                      32 },
                                                                                                                    .text =
                                                                                                                        "Quit to Main Menu?",
                                                                                                                    .color =
                                                                                                                        Color::
                                                                                                                            black }
                                                                                                            )
                                                                                                                .SetIsIdealSize(
                                                                                                                )
                                                                                                                .HStack({ .spacing =
                                                                                                                              100,
                                                                                                                          .buildViewFunc =
                                                                                                                              [](auto&
                                                                                                                                     vb
                                                                                                                              ) {
                                                                                                                                  vb.Text(
                                                                                                                                        { .fontSpec = { .size =
                                                                                                                                                            32 },
                                                                                                                                          .text =
                                                                                                                                              "NO",
                                                                                                                                          .color =
                                                                                                                                              Color::
                                                                                                                                                  white }
                                                                                                                                  )
                                                                                                                                      .SetIsIdealSize(
                                                                                                                                      )
                                                                                                                                      .Text(
                                                                                                                                          { .fontSpec = { .size =
                                                                                                                                                              32 },
                                                                                                                                            .text =
                                                                                                                                                "YES",
                                                                                                                                            .color =
                                                                                                                                                Color::
                                                                                                                                                    white }
                                                                                                                                      )
                                                                                                                                      .SetIsIdealSize(
                                                                                                                                      );
                                                                                                                              } }
                                                                                                                );
                                                                                                        } }
                                                                                          )
                                                                                          .Spacer();
                                                                                  } });
                                                              } });
                                                 } })
                              .Spacer();
                      } });
                  });
              } }
        );

        buildViewModels.push_back({ "Nuclear Throne", [=](WorldNode& root) {
                                       QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                                           vb.ZStack({ .buildViewFunc = [](auto& vb) {
                                               vb.Color(Color::black)
                                                   .Pad(
                                                       { .name = "Outer pad",
                                                         .insets = LayoutInsets::Uniform(10),
                                                         .buildViewFunc =
                                                             [](auto& vb) {
                                                                 vb.VStack({ .name = "Scree"
                                                                                     "n "
                                                                                     "stac"
                                                                                     "k",
                                                                             .buildViewFunc =
                                                                                 [](auto& vb) {
                                                                                     vb.HStack(
                                                                                           { .name =
                                                                                                 "T"
                                                                                                 "o"
                                                                                                 "p"
                                                                                                 " "
                                                                                                 "s"
                                                                                                 "t"
                                                                                                 "a"
                                                                                                 "c"
                                                                                                 "k",
                                                                                             .buildViewFunc =
                                                                                                 [](auto&
                                                                                                        vb
                                                                                                 ) {
                                                                                                     vb
                                                                                                         .HStack(
                                                                                                             { .name =
                                                                                                                   "Left section",
                                                                                                               .buildViewFunc =
                                                                                                                   [](auto&
                                                                                                                          vb
                                                                                                                   ) {
                                                                                                                       vb.HStack(
                                                                                                                           { .spacing =
                                                                                                                                 10,
                                                                                                                             .alignFunc =
                                                                                                                                 AlignFuncs::
                                                                                                                                     left,
                                                                                                                             .buildViewFunc =
                                                                                                                                 [](auto&
                                                                                                                                        vb
                                                                                                                                 ) {
                                                                                                                                     vb.Color(
                                                                                                                                           Color::
                                                                                                                                               green
                                                                                                                                     )
                                                                                                                                         .FixedSize(
                                                                                                                                             20,
                                                                                                                                             50
                                                                                                                                         )
                                                                                                                                         .VStack(
                                                                                                                                             { .spacing =
                                                                                                                                                   5,
                                                                                                                                               .buildViewFunc =
                                                                                                                                                   [](auto&
                                                                                                                                                          vb
                                                                                                                                                   ) {
                                                                                                                                                       vb.Color(
                                                                                                                                                             Color::
                                                                                                                                                                 red
                                                                                                                                                       )
                                                                                                                                                           .FixedSize(
                                                                                                                                                               100,
                                                                                                                                                               20
                                                                                                                                                           )
                                                                                                                                                           .HStack(
                                                                                                                                                               { .spacing =
                                                                                                                                                                     5,
                                                                                                                                                                 .alignFunc =
                                                                                                                                                                     AlignFuncs::
                                                                                                                                                                         left,
                                                                                                                                                                 .buildViewFunc =
                                                                                                                                                                     [](auto&
                                                                                                                                                                            vb
                                                                                                                                                                     ) {
                                                                                                                                                                         vb.Color(
                                                                                                                                                                               Color::
                                                                                                                                                                                   gray
                                                                                                                                                                         )
                                                                                                                                                                             .FixedSize(
                                                                                                                                                                                 30,
                                                                                                                                                                                 20
                                                                                                                                                                             )
                                                                                                                                                                             .Text(
                                                                                                                                                                                 { .fontSpec = { .size =
                                                                                                                                                                                                     32 },
                                                                                                                                                                                   .text =
                                                                                                                                                                                       "50",
                                                                                                                                                                                   .color =
                                                                                                                                                                                       Color::
                                                                                                                                                                                           white }
                                                                                                                                                                             )
                                                                                                                                                                             .FixedSize(
                                                                                                                                                                                 50,
                                                                                                                                                                                 40
                                                                                                                                                                             );
                                                                                                                                                                     } }
                                                                                                                                                           );
                                                                                                                                                   } }
                                                                                                                                         );
                                                                                                                                 } }
                                                                                                                       );
                                                                                                                   } }
                                                                                                         )
                                                                                                         .Spacer(
                                                                                                         )
                                                                                                         .HStack({ .name = "Right section", .buildViewFunc = [](auto& vb) {
                                                                                                                      vb.HStack(
                                                                                                                          { .spacing =
                                                                                                                                5,
                                                                                                                            .buildViewFunc =
                                                                                                                                [](auto&
                                                                                                                                       vb
                                                                                                                                ) {
                                                                                                                                    vb.Color(
                                                                                                                                          Color::
                                                                                                                                              blue
                                                                                                                                    )
                                                                                                                                        .FixedSize(
                                                                                                                                            30,
                                                                                                                                            30
                                                                                                                                        )
                                                                                                                                        .Color(
                                                                                                                                            Color::
                                                                                                                                                blue
                                                                                                                                        )
                                                                                                                                        .FixedSize(
                                                                                                                                            30,
                                                                                                                                            30
                                                                                                                                        );
                                                                                                                                } }
                                                                                                                      );
                                                                                                                  } });
                                                                                                 } }
                                                                                     ).Spacer();
                                                                                 } });
                                                             } }
                                                   );
                                           } });
                                       });
                                   } });

        buildViewModels.push_back(
            { "0.1",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [](auto& vb) {
                          vb.Color(Color::red)
                              .Pad({ .insets = LayoutInsets::Uniform(20),
                                     .buildViewFunc = [](auto& vb) { vb.Color(Color::blue); } })
                              .Text({ .fontSpec = { .size = 32 },
                                      .text = "Hello World",
                                      .color = Color::black });
                          //                                    .FixedSize(400,
                          //                                    400);
                      } });
                  });
              } }
        );
        buildViewModels.push_back(
            { "0.2",
              [=](WorldNode& root) {
                  QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                      vb.ZStack({ .buildViewFunc = [](auto& vb) {
                          vb.Color(Color::red)
                              .VStack({ .spacing = 10, .buildViewFunc = [](auto& vb) {
                                           vb.HStack(
                                                 { .spacing = 10,
                                                   .buildViewFunc =
                                                       [](auto& vb) {
                                                           vb.Color(Color::blue)
                                                               .FixedSize(30, 30)
                                                               .Color(Color::blue)
                                                               .FixedSize(20, 20)
                                                               .Pad({ .insets =
                                                                          LayoutInsets::Uniform(10),
                                                                      .buildViewFunc = [](auto& vb
                                                                                       ) {
                                                                          vb.Color(Color::black);
                                                                      } });
                                                       } }
                                           ).Color(Color::gray);
                                       } });
                      } });
                  });
              } }
        );

        RebuildTargetView();
}

void TestViewsScene::RebuildTargetView() {
        GUARD(targetView >= 0 && targetView < buildViewModels.size())
        GUARD(owner)

        owner->RemoveAllChildren();
        buildViewModels[targetView].buildViewFunc(*owner);
        auto rootView = RootView();
        if (rootView) {
            rootView->SetEnvironmentValue("color", Color(Color32(33, 60, 89)));
        }
}
