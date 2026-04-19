#include "ImGuiMainMenuBarPainter.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

ImGuiPaintMainMenuBar::ImGuiPaintMainMenuBar(std::function<void()> func) {
    if (ImGui::BeginMainMenuBar()) {
        if (func) {
            func();
        }

        ImGui::EndMainMenuBar();
    }
}

ImGuiMainMenuBarPainter::ImGuiMainMenuBarPainter(VectorList<UP<Menu>> const& menus) :
    menus(menus) {
    drawFunc = [](auto& _painter) {
        auto& painter = *(static_cast<This*>(&_painter));
        GUARD(!painter.menus.empty())

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
