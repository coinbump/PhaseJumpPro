#pragma once

#include "imgui.h"
#include "Menu.h"
#include "SomeImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    /// Handles both begin and end. Simplifies using imGui
    struct ImGuiPaintMainMenuBar {
        ImGuiPaintMainMenuBar(std::function<void()> func) {
            if (ImGui::BeginMainMenuBar()) {
                if (func) {
                    func();
                }

                ImGui::EndMainMenuBar();
            }
        }
    };

    /// Builds an imGui main menu from PhaseJump menu objects
    class ImGuiMainMenuBarPainter : public SomeImGuiPainter {
    public:
        using Base = SomeImGuiPainter;
        using This = ImGuiMainMenuBarPainter;

        VectorList<UP<Menu>> const& menus;

        ImGuiMainMenuBarPainter(VectorList<UP<Menu>> const& menus) :
            menus(menus) {
            drawFunc = [](auto& _painter) {
                auto& painter = *(static_cast<This*>(&_painter));

                ImGuiPaintMainMenuBar mainMenu([&]() {
                    for (auto& menu : painter.menus) {
                        if (ImGui::BeginMenu(menu->title.c_str())) {
                            for (auto& _menuItem : menu->items) {
                                if (Is<SeparatorMenuItem>(_menuItem.get())) {
                                    ImGui::Separator();
                                    continue;
                                }

                                auto menuItem = As<MenuItem>(_menuItem.get());
                                if (menuItem) {
                                    String shortcutString;
                                    VectorList<String> shortcutStrings;
                                    std::for_each(
                                        menuItem->shortcuts.begin(), menuItem->shortcuts.end(),
                                        [&](auto& shortcut) {
                                            shortcutStrings.push_back(shortcut.ToString());
                                        }
                                    );
                                    shortcutString = Joined(shortcutStrings, ",");

                                    if (ImGui::MenuItem(
                                            menuItem->Title().c_str(), shortcutString.c_str(),
                                            menuItem->IsToggleOn(), menuItem->IsEnabled()
                                        )) {
                                        menuItem->Run();
                                    }
                                }
                            }

                            ImGui::EndMenu();
                        }
                    }
                });
            };
        }
    };
} // namespace PJ
