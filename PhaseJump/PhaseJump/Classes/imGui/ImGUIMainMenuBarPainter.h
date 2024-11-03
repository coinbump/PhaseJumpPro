#pragma once

#include "imgui.h"
#include "Menu.h"
#include "SomeImGuiPainter.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/14/23
 */
namespace PJ {
    // TODO: rename this? Naming is weird
    /// Handles both begin and end. Simplifies using imGui
    struct ImGuiBeginMainMenuBar {
        ImGuiBeginMainMenuBar(std::function<void()> func) {
            if (ImGui::BeginMainMenuBar()) {
                if (func) {
                    func();
                }

                // TODO: handle this in destructor instead?
                ImGui::EndMainMenuBar();
            }
        }
    };

    /// Provides a value and an optional alternate storage for that value
    //    template <class Type>
    //    class Storable {
    //    public:
    //        Type value{};
    //        Type* storage{};
    //
    //        Storable(Type value, Type* storage) : value(value), storage(storage) {
    //        }
    //
    //        Type* Storage() {
    //            return storage ? storage : &value;
    //        }
    //    };

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

                ImGuiBeginMainMenuBar mainMenu([&]() {
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
            };
        }
    };
} // namespace PJ
