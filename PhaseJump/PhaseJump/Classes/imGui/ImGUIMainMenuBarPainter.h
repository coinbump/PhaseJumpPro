#pragma once

#include "imgui.h"
#include "ImGuiPainter.h"
#include "Menu.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/19/24
 */
namespace PJ {
    /// Handles both begin and end. Simplifies using imGui
    struct ImGuiPaintMainMenuBar {
        ImGuiPaintMainMenuBar(std::function<void()> func);
    };

    /// Builds an imGui main menu from PhaseJump menu objects
    class ImGuiMainMenuBarPainter : public ImGuiPainter {
    public:
        using Base = ImGuiPainter;
        using This = ImGuiMainMenuBarPainter;

        VectorList<UP<Menu>> const& menus;

        ImGuiMainMenuBarPainter(VectorList<UP<Menu>> const& menus);
    };
} // namespace PJ
