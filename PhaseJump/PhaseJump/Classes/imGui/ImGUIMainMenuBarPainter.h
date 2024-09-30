#pragma once

#include "imgui.h"
#include "Menu.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Handles both begin and end. Simplifies using imGui
    struct ImGUIBeginMainMenuBar {
        ImGUIBeginMainMenuBar(std::function<void()> func) {
            if (ImGui::BeginMainMenuBar()) {
                if (func) {
                    func();
                }

                ImGui::EndMainMenuBar();
            }
        }
    };

    /// Builds an imGui main menu from PhaseJump menu objects
    struct ImGUIMainMenuBarPainter {
        void Draw(VectorList<SP<Menu>> const& menus) {
            ImGUIBeginMainMenuBar mainMenu([&]() {
                for (auto& menu : menus) {
                    if (ImGui::BeginMenu(menu->title.c_str())) {
                        for (auto& _menuItem : menu->items) {
                            if (Is<SeparatorMenuItem>(_menuItem.get())) {
                                ImGui::Separator();
                                continue;
                            }

                            auto menuItem = As<MenuItem>(_menuItem.get());
                            if (menuItem) {
                                String shortcutString;
                                List<String> shortcutStrings;
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
        }
    };
} // namespace PJ
