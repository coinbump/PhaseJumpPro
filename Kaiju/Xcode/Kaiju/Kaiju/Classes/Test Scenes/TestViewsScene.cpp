#include "TestViewsScene.h"

using namespace std;
using namespace PJ;

TestViewsScene::TestViewsScene() {
    layouts = { { .id = "1080", .size = { 1920, 1080 } },
                { .id = "phone", .size = { 750, 1334 } } };

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        Binding<int> binding({ .getFunc =
                                   [this]() { return LayoutSizeIndex() ? *LayoutSizeIndex() : -1; },
                               .setFunc = [this](auto& value) { SetLayoutSizeIndex(value); } });
        planner.ListSelect({ .label = "Sizes", .options = { "1080", "phone" }, .binding = binding }
        );

        planner.PickerColor(
            { .label = "Environment Color",
              .binding = Binding<Color>({ .getFunc =
                                              [this]() {
                                                  auto rootView = RootView();
                                                  return rootView
                                                             ? rootView->EnvironmentValue<Color>(
                                                                   "color", Color::gray
                                                               )
                                                             : Color::gray;
                                              },
                                          .setFunc =
                                              [this](auto& value) {
                                                  auto rootView = RootView();
                                                  if (rootView) {
                                                      rootView->SetEnvironmentValue("color", value);
                                                  }
                                              } }) }
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
    root.SetName("TestViewsScene");

    Vector2 _1080{ 1920, 1080 };

    QB(root).OrthoStandard();

    auto& ac = root.With<WorldComponent>();
    rootComponent = &ac;
    ac.AddSignalHandler<KeyDownUIEvent>({ .id = SignalId::KeyDown,
                                          .func = [this,
                                                   &root](auto& component, auto& keyDownEvent) {
                                              switch (keyDownEvent.KeyCodeValue()) {
                                              case SDLK_LEFT:
                                                  {
                                                      targetView--;
                                                      if (targetView < 0) {
                                                          targetView =
                                                              (int)buildViewModels.size() - 1;
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
                                          } });

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
            { .isOnBinding = Binding<bool>(
                  { .getFunc = [=]() { return toggleGroupStore->selection.Value() == id; },
                    .setFunc =
                        [=](auto& value) {
                            if (value)
                                toggleGroupStore->selection = id;
                        } }
              ),
              .onViewStateChangeFunc =
                  [](auto& control) {
                      auto& button = *(static_cast<ToggleButtonControl*>(&control));
                      GUARD(!IsEmpty(button.owner->Children()))

                      auto renderer = button.owner->Children()[0]->TypeComponent<Renderer>();
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
                      auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                      textView(vb, label);
                  } }
        );

        return vb;
    };

    DuckDesignSystem duck;

    auto duckUIStack = [=, this](ViewBuilder& vb) {
        auto v = vb.BeginVStack({ .spacing = 20 });

        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Label + Color" });
            vb.Label({ .color = Color::red, .text = "Red Label" });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Button" });
            vb.Button({ .label = "Duck UI" });
        }
        {
            auto h = vb.BeginHStack({ .spacing = 15 });
            vb.Label({ .text = "Dial" });
            vb.Dial({ .valueBinding = Binding<float>({ .getFunc = [this]() { return dialValue; },
                                                       .setFunc = [this](auto& value
                                                                  ) { dialValue = value; } }) });
            vb.Dial({ .surfaceColor = Color32(106, 162, 137),
                      .valueBinding = Binding<float>({ .getFunc = [this]() { return dialValue2; },
                                                       .setFunc = [this](auto& value
                                                                  ) { dialValue2 = value; } }) });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Progress Bar" });
            vb.ProgressBar({ .valueFunc = []() { return 0.3f; } });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Progress Bar + Color" });
            vb.ProgressBar({ .progressColor = Color::red, .valueFunc = []() { return 0.3f; } });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Progress Circle" });
            vb.ProgressCircle({ .valueFunc = []() { return 0.65f; } });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Segment Toggle" });
            vb.SegmentToggle({ .label = "Toggle" });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Check Button" });
            vb.CheckButton({ .label = "Checked" });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Radio Button Group" });
            vb.RadioButtonGroup({ .store = toggleGroupStore, .options = { "Option 1", "Option 2" } }
            );
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Segmented Picker" });
            vb.SegmentedPicker({ .store = toggleGroupStore,
                                 .options = { "First", "Second", "Final" } });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Image Toggle" });
            vb.ImageToggle({ .imageId = "heart-full", .size = Vector2{ 100, 100 } });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Switch Toggle" });
            vb.SwitchToggle({});
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "Toast" });
            vb.Toast({ .text = "This is an important announcement" });
        }
        {
            auto h = vb.BeginHStack({});
            vb.Label({ .text = "ToolTip" });
            vb.Button({ .label = "Hover for Tip", .modifyViewFunc = [=](auto& view) {
                           ViewBuilder(*view.owner)
                               .AddToolTip({ .text = "This is a helpful ToolTip" });
                       } });
        }
        {
            auto h = vb.BeginHStack({});
            // FUTURE: add vertical slider if needed
            vb.Label({ .text = "Slider" });
            vb.Slider({ .axis = Axis2D::X });
        }
    };

    auto colorRect = [](ViewBuilder& vb, String colorId, String colorName) {
        auto designSystem = vb.GetDesignSystem();

        vb.Immediate(
              { .renderFunc =
                    [=](auto& view, auto& renderer) {
                        renderer.areShapesOpaque = true;
                        renderer.SetColor(designSystem->theme->ThemeColor(colorId, Color::gray));

                        renderer.FillRect(view.Bounds())
                            .Text(colorName, Vector2{ 10, 10 }, 32, Color::black);
                    } }
        ).FixedSize({}, 36);
    };

    auto colorsStack = [=](ViewBuilder& vb) {
        auto designSystem = vb.GetDesignSystem();

        auto v = vb.BeginVStack({ .spacing = 5 });
        for (auto& colorItem : designSystem->theme->Colors()) {
            colorRect(vb, colorItem.first, colorItem.first);
        }
    };

    buildViewModels.push_back({ "Duck", [=](WorldNode& root) {
                                   QB(root)
                                       .And("Root View")
                                       .RootView(
                                           { 800, 1800 },
                                           [=](auto& vb) {
                                               auto z = vb.BeginZStack({});
                                               vb.Surface();
                                               auto p =
                                                   vb.BeginPad({ .insets =
                                                                     LayoutInsets::Uniform(20) });
                                               duckUIStack(vb);
                                           }
                                       )
                                       .SizeWithWindow();
                               } });

    buildViewModels.push_back({ "Color roles", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView({ 800, 1800 }, [=](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Surface();
                                       auto p =
                                           vb.BeginPad({ .insets = LayoutInsets::Uniform(20) });
                                       colorsStack(vb);
                                   });
                               } });

    buildViewModels.push_back(
        { "Pop",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                  auto z = vb.BeginZStack({});
                  vb.Color(Color32(33, 60, 89));
                  auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                  auto h = vb.BeginHStack({ .spacing = 20 });
                  auto view = vb.ActiveView();

                  colorButton(vb, "Pop", [view](auto& button) {
                      ViewBuilder vb(*view->owner);
                      vb.ActiveView()->Present(
                          { .makeFrameFunc =
                                [](auto& presentingView, auto& view) {
                                    Vector2 popoverPosition{ 0, presentingView.Frame().size.y };
                                    return Rect{ .origin = presentingView.ToRootViewPosition(
                                                     popoverPosition
                                                 ),
                                                 .size = { 200, 100 } };
                                },
                            .buildViewFunc =
                                [](auto& vb) {
                                    vb.Immediate({ .renderFunc = [](auto& view, auto& renderer) {
                                        renderer.FillRect(view.Bounds());
                                    } });
                                } }
                      );
                  });

                  vb.Button({ .label = "Button", .modifyViewFunc = [](auto& view) {
                                 ViewBuilder(*view.owner)
                                     .AddToolTip({ .text = "This is a test of a very long "
                                                           "tooltip, what happens when the "
                                                           "text gets longer, does it wrap "
                                                           "to a new line or does something "
                                                           "terrible happen, and then if it "
                                                           "gets longer and longer and "
                                                           "longer what gives?" });
                             } });

                  colorButton(vb, "Button", {});
              });
          } }
    );

    auto fixedGridView = [=, this](ViewBuilder& vb) {
        auto g = vb.BeginFixedGrid({ .gridSize = { 5, 5 }, .spacing = { 20, 20 } });
        Repeat(15, [this, &vb] {
            auto z = vb.BeginZStack({});
            vb.Immediate({ .renderFunc =
                               [](auto& view, auto& renderer) {
                                   renderer.SetStrokeColor(Color::white)
                                       .SetStrokeWidth(3)
                                       .FrameRect(view.Bounds());
                               } }
            ).FixedSize(100, 100);

            vb.Dial({ .valueBinding = Binding<float>({ .getFunc = [this]() { return dialValue; },
                                                       .setFunc = [this](auto& value
                                                                  ) { dialValue = value; } }) });
        });
    };

    buildViewModels
        .push_back({ "FixedGrid", [=](WorldNode& root) {
                        QB(root).And("Root View").RootView({ 1000, 1000 }, [=](auto& vb) {
                            auto z = vb.BeginZStack({});
                            vb.Surface();
                            auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(20) });
                            fixedGridView(vb);
                        });
                    } });

    buildViewModels.push_back({ "Immediate", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView({ 800, 800 }, [=](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Color(Color::white);
                                       vb.Immediate({ .renderFunc = [](auto& view, auto& renderer) {
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
                                   });
                               } });

    buildViewModels.push_back({ "Sliders-bug", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Color(Color32(33, 60, 89));
                                       auto p =
                                           vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                                       auto outer = vb.BeginHStack({});
                                       {
                                           auto left =
                                               vb.BeginHStack({ .spacing = 10,
                                                                .alignFunc = AlignFuncs::left });
                                           for (int i = 0; i < 1; i++) {
                                               vb.SliderView(ViewBuilder::SliderViewConfig{});
                                           }
                                       }
                                       {
                                           auto right = vb.BeginHStack({ .spacing = 10 });
                                           for (int i = 0; i < 1; i++) {
                                               vb.SliderView({ .axis = Axis2D::Y });
                                           }
                                       }
                                   });
                               } });

    auto strayHStack = [=](ViewBuilder& vb) -> ViewBuilder& {
        auto outer = vb.BeginHStack({ .spacing = 0 });
        vb.Spacer();
        {
            auto v = vb.BeginVStack({ .spacing = 40 });
            vb.Text({ .fontSpec = { .size = 32 },
                      .text = AttributedString("Quit to Main Menu?")
                                  .ApplyColor(Color::red, 0, 4)
                                  .ApplyColor(Color::blue, 8, 9),
                      .color = Color::black })
                .SetIsIdealSize();
            {
                auto h = vb.BeginHStack({ .spacing = 100 });
                vb.Text({ .fontSpec = { .size = 32 }, .text = "NO", .color = Color::white })
                    .SetIsIdealSize();
                vb.Text({ .fontSpec = { .size = 32 }, .text = "YES", .color = Color::white })
                    .SetIsIdealSize();
            }
        }
        vb.Spacer();

        return vb;
    };

    buildViewModels.push_back({ "Stray-v2", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                                       auto v = vb.BeginVStack({});
                                       vb.Spacer();
                                       {
                                           auto z = vb.BeginZStack({});
                                           // Pad + Background is chained, use callback form
                                           vb.Pad({ .insets = LayoutInsets::Uniform(50),
                                                    .buildViewFunc = [=](auto& vb
                                                                     ) { strayHStack(vb); } }
                                           ).Background([=](auto& vb) {
                                               vb.Color(Color(1, 137.0f / 255.0f, 0, 0.9f));
                                           });
                                       }
                                       vb.Spacer();
                                   });
                               } });

    buildViewModels.push_back({ "Toggle Button Group", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Color(Color32(33, 60, 89));
                                       auto p =
                                           vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                                       auto h = vb.BeginHStack({ .spacing = 20 });
                                       toggleButtonGroup(vb, "1", "Toggle");
                                       toggleButtonGroup(vb, "2", "Toggle");
                                       toggleButtonGroup(vb, "3", "Toggle");
                                   });
                               } });

    buildViewModels.push_back(
        { "Spinner",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                  auto z = vb.BeginZStack({});
                  vb.Color([](auto& view) {
                      return ViewBuilder::ColorConfig{
                          .color = view.template EnvironmentValue<Color>("color")
                      };
                  });
                  auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                  auto h = vb.BeginHStack({ .spacing = 20 });

                  // Children depend on spinnerValue — make the HStack rebuildable
                  vb.Rebuildable([=, spinnerValue = 10](ViewBuilder& vb) mutable {
                      colorButton(vb, "L", [&](auto& button) {
                          spinnerValue--;
                          button.owner->Parent()->template TypeComponent<View2D>()->SetNeedsRebuild(
                          );
                      });
                      textView(vb, MakeString(spinnerValue)).FixedSize(300, {});
                      colorButton(vb, "R", [&](auto& button) {
                          spinnerValue++;
                          button.owner->Parent()->template TypeComponent<View2D>()->SetNeedsRebuild(
                          );
                      });
                  });
              });
          } }
    );

    buildViewModels.push_back(
        { "Toggle Buttons",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView({ 800, 200 }, [=](auto& vb) {
                  auto z = vb.BeginZStack({});
                  vb.Color(Color32(33, 60, 89));
                  auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                  // Dynamic HStack config reads from environment — use callback form
                  vb.HStack([=](View2D& view) {
                      return ViewBuilder::HStackConfig{
                          .spacing = view.EnvironmentValue<Color>("color").r * 255,
                          .buildViewFunc =
                              [=](auto& vb) {
                                  toggleButton(vb, "Toggle");
                                  toggleButton(vb, "Toggle");

                                  if (vb.ActiveView()->template EnvironmentValue<Color>("color").r >
                                      0.5) {
                                      toggleButton(vb, "Extra");
                                  }
                              }
                      };
                  });
              });
          } }
    );

    static SP<SingleSelectStore> pageViewStore = MAKE<SingleSelectStore>();

    buildViewModels.push_back(
        { "Pages",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                  auto z = vb.BeginZStack({});
                  vb.Color(Color32(33, 60, 89));
                  auto v = vb.BeginVStack({});
                  {
                      auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                      auto h = vb.BeginHStack({ .spacing = 20 });
                      colorButton(vb, "Page 1", [](auto& button) {
                          pageViewStore->selection = "1";
                      });
                      colorButton(vb, "Page 2", [](auto& button) {
                          pageViewStore->selection = "2";
                      });
                  }
                  vb.Pages({ .store = pageViewStore,
                             .selectedPage = "2",
                             .pages = { { .id = "1",
                                          .buildPageFunc =
                                              [=](auto& node) {
                                                  ViewBuilder vb(node);
                                                  textView(vb, "1");
                                              } },
                                        { .id = "2", .buildPageFunc = [=](auto& node) {
                                             ViewBuilder vb(node);
                                             textView(vb, "2");
                                         } } } });
                  vb.QB().template ModifyLatest<PageView>([](auto& view) {
                      view.AddSignalHandler(
                          { .id = SignalId::KeyDown,
                            .func =
                                [&](auto& component, auto& signal) {
                                    auto keyDownEvent =
                                        dynamic_cast<KeyDownUIEvent const*>(&signal);
                                    GUARD(keyDownEvent)

                                    switch (keyDownEvent->KeyCodeValue()) {
                                    case '8':
                                        view.Navigate(PageView::NavigateDirection::Back, true);
                                        break;
                                    case '9':
                                        view.Navigate(PageView::NavigateDirection::Forward, true);
                                        break;
                                    }
                                } }
                      );
                  });
              });
          } }
    );

    buildViewModels.push_back({ "Sliders", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Color(Color32(33, 60, 89));
                                       auto p =
                                           vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                                       auto outer = vb.BeginHStack({});
                                       {
                                           auto left =
                                               vb.BeginVStack({ .spacing = 10,
                                                                .alignFunc = AlignFuncs::left });
                                           for (int i = 0; i < 4; i++) {
                                               vb.SliderView(ViewBuilder::SliderViewConfig{});
                                           }
                                       }
                                       {
                                           auto right = vb.BeginHStack({ .spacing = 10 });
                                           for (int i = 0; i < 4; i++) {
                                               vb.SliderView({ .axis = Axis2D::Y });
                                           }
                                       }
                                   });
                               } });

    buildViewModels.push_back({ "Alex Kidd", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Color(Color::yellow);
                                       auto p =
                                           vb.BeginPad({ .insets = LayoutInsets::Uniform(20) });
                                       auto outer = vb.BeginHStack({});
                                       vb.Spacer();
                                       {
                                           auto v = vb.BeginVStack({ .spacing = 30 });
                                           textView(vb, "LOAD GAME");
                                           textView(vb, "NEW GAME");
                                           {
                                               auto inner = vb.BeginVStack({ .spacing = 5 });
                                               textView(vb, "CLASSIC MODE");
                                               textView(vb, "BOSS RUSH");
                                           }
                                           {
                                               auto inner = vb.BeginVStack({ .spacing = 5 });
                                               textView(vb, "OPTIONS");
                                               textView(vb, "CREDITS");
                                               textView(vb, "EXIT");
                                           }
                                       }
                                   });
                               } });

    // .Offset targets the HStack after the scope closes, which works because Scope::Leave()
    // truncates the component list back to the scope's own view.
    auto anthemButton = [=](ViewBuilder& vb, String text, int index) -> ViewBuilder& {
        {
            auto h = vb.BeginHStack({ .spacing = 10 });
            vb.Color(Color32(242, 241, 211)).FixedSize(20, 20);
            vb.Text({ .fontSpec = { .size = 32 }, .text = text, .color = Color32(242, 241, 211) })
                .SetIsIdealSize();
        }
        vb.Offset(index * 10, 0);

        return vb;
    };

    buildViewModels.push_back(
        { "Anthem",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                  auto z = vb.BeginZStack({});
                  vb.Color(Color32(33, 60, 89));
                  auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(10) });
                  auto outer = vb.BeginHStack({});
                  {
                      auto v = vb.BeginVStack({ .alignFunc = AlignFuncs::left, .spacing = 10 });
                      VectorList<String> buttons{ "GAMEPLAY", "CONTROLS",  "AUDIO",
                                                  "VIDEO",    "INTERFACE", "ACCESSIBILITY",
                                                  "ACCOUNT",  "OTHER",     "QUIT GAME" };

                      for (int i = 0; i < buttons.size(); i++) {
                          anthemButton(vb, buttons[i], i);
                      }
                  }
                  vb.Spacer();
              });
          } }
    );

    buildViewModels.push_back({ "Stray- no VStack", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                                       auto outer = vb.BeginVStack({});
                                       vb.Spacer();
                                       {
                                           auto z = vb.BeginZStack({});
                                           vb.Color(Color(1, 137.0f / 255.0f, 0, 1));
                                           auto p =
                                               vb.BeginPad({ .insets = LayoutInsets::Uniform(50) });
                                           auto h = vb.BeginHStack({ .spacing = 100 });
                                           vb.Text({ .fontSpec = { .size = 32 },
                                                     .text = "YES",
                                                     .color = Color::white })
                                               .SetIsIdealSize();
                                       }
                                       vb.Spacer();
                                   });
                               } });

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

    buildViewModels.push_back(
        { "Text Stack",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                  auto h = vb.BeginHStack({});
                  vb.Text({ .fontSpec = { .size = 32 }, .text = lorem, .color = Color::black });
                  vb.Text({ .fontSpec = { .size = 32 }, .text = lorem, .color = Color::black });
              });
          } }
    );

    buildViewModels.push_back(
        { "Stray",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                  auto outer = vb.BeginVStack({});
                  vb.Spacer();
                  // ViewAttachments has multi-child semantics (bg + content + overlay) — keep
                  // the callback form. Its sub-builders can use RAII internally.
                  vb.ViewAttachments(
                      { .buildBackgroundFunc = [](auto& vb
                                               ) { vb.Color(Color(1, 137.0f / 255.0f, 0, 1)); },
                        .buildViewFunc =
                            [](auto& vb) {
                                auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(50) });
                                auto h = vb.BeginHStack({ .spacing = 0 });
                                vb.Spacer();
                                {
                                    auto v = vb.BeginVStack({ .spacing = 40 });
                                    vb.Text({ .fontSpec = { .size = 32 },
                                              .text = "Quit to Main Menu?",
                                              .color = Color::black })
                                        .SetIsIdealSize();
                                    {
                                        auto inner = vb.BeginHStack({ .spacing = 100 });
                                        vb.Text({ .fontSpec = { .size = 32 },
                                                  .text = "NO",
                                                  .color = Color::white })
                                            .SetIsIdealSize();
                                        vb.Text({ .fontSpec = { .size = 32 },
                                                  .text = "YES",
                                                  .color = Color::white })
                                            .SetIsIdealSize();
                                    }
                                }
                                vb.Spacer();
                            } }
                  );
                  vb.Spacer();
              });
          } }
    );

    buildViewModels.push_back(
        { "Nuclear Throne",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                  auto z = vb.BeginZStack({});
                  vb.Color(Color::black);
                  auto outer =
                      vb.BeginPad({ .name = "Outer pad", .insets = LayoutInsets::Uniform(10) });
                  auto screen = vb.BeginVStack({ .name = "Screen stack" });
                  {
                      auto top = vb.BeginHStack({ .name = "Top stack" });
                      {
                          auto left = vb.BeginHStack({ .name = "Left section" });
                          {
                              auto row =
                                  vb.BeginHStack({ .spacing = 10, .alignFunc = AlignFuncs::left });
                              vb.Color(Color::green).FixedSize(20, 50);
                              {
                                  auto v = vb.BeginVStack({ .spacing = 5 });
                                  vb.Color(Color::red).FixedSize(100, 20);
                                  {
                                      auto inner =
                                          vb.BeginHStack({ .spacing = 5,
                                                           .alignFunc = AlignFuncs::left });
                                      vb.Color(Color::gray).FixedSize(30, 20);
                                      vb.Text({ .fontSpec = { .size = 32 },
                                                .text = "50",
                                                .color = Color::white })
                                          .FixedSize(50, 40);
                                  }
                              }
                          }
                      }
                      vb.Spacer();
                      {
                          auto right = vb.BeginHStack({ .name = "Right section" });
                          auto inner = vb.BeginHStack({ .spacing = 5 });
                          vb.Color(Color::blue).FixedSize(30, 30);
                          vb.Color(Color::blue).FixedSize(30, 30);
                      }
                  }
                  vb.Spacer();
              });
          } }
    );

    buildViewModels.push_back({ "0.1", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Color(Color::red);
                                       {
                                           auto p =
                                               vb.BeginPad({ .insets = LayoutInsets::Uniform(20) });
                                           vb.Color(Color::blue);
                                       }
                                       vb.Text({ .fontSpec = { .size = 32 },
                                                 .text = "Hello World",
                                                 .color = Color::black });
                                   });
                               } });

    buildViewModels.push_back({ "0.2", [=](WorldNode& root) {
                                   QB(root).And("Root View").RootView(_1080, [](auto& vb) {
                                       auto z = vb.BeginZStack({});
                                       vb.Color(Color::red);
                                       {
                                           auto v = vb.BeginVStack({ .spacing = 10 });
                                           {
                                               auto h = vb.BeginHStack({ .spacing = 10 });
                                               vb.Color(Color::blue).FixedSize(30, 30);
                                               vb.Color(Color::blue).FixedSize(20, 20);
                                               {
                                                   auto p = vb.BeginPad(
                                                       { .insets = LayoutInsets::Uniform(10) }
                                                   );
                                                   vb.Color(Color::black);
                                               }
                                           }
                                           vb.Color(Color::gray);
                                       }
                                   });
                               } });

    buildViewModels.push_back(
        { "Background + Overlay",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                  // Pad + chained Background + Overlay needs the callback form
                  vb.Pad({ .insets = LayoutInsets::Uniform(50),
                           .buildViewFunc =
                               [](auto& vb) {
                                   vb.Text({ .fontSpec = { .size = 32 },
                                             .text = "Background/Overlay",
                                             .color = Color::black })
                                       .SetIsIdealSize();
                               } }
                  ).Background([](auto& vb) {
                       vb.Color(Color(1, 0, 137.0f / 255.0f, 1));
                   }).Overlay([](auto& vb) { vb.Color(Color(1, 1, 1, 0.3f)); });
              });
          } }
    );

    buildViewModels.push_back(
        { "View Attachments",
          [=](WorldNode& root) {
              QB(root).And("Root View").RootView(_1080, [=](auto& vb) {
                  vb.ViewAttachments(
                      { .buildBackgroundFunc = [](auto& vb
                                               ) { vb.Color(Color(1, 137.0f / 255.0f, 0, 1)); },
                        .buildViewFunc =
                            [](auto& vb) {
                                auto p = vb.BeginPad({ .insets = LayoutInsets::Uniform(50) });
                                vb.Text({ .fontSpec = { .size = 32 },
                                          .text = "Quit to Main Menu?",
                                          .color = Color::black })
                                    .SetIsIdealSize();
                            } }
                  );
              });
          } }
    );

    RebuildTargetView();
}

void TestViewsScene::RebuildTargetView() {
    GUARD(targetView >= 0 && targetView < buildViewModels.size())
    GUARD(owner)

    owner->DestroyAllChildren();
    buildViewModels[targetView].buildViewFunc(*owner);
    auto rootView = RootView();
    if (rootView) {
        rootView->SetEnvironmentValue("color", Color(Color32(33, 60, 89)));
    }
}
