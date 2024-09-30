#pragma once

#include "Color.h"
#include "imgui.h"
#include "UIPlan.h"
#include "UnorderedMap.h"

namespace PJ {
    /// Paints a UI plan into imGui
    class ImGuiPlanPainter {
    public:
        using DrawFunc = std::function<void(SomeUIModel&)>;

        UnorderedMap<String, DrawFunc> drawFuncs;

        ImGuiPlanPainter();

        void Draw(UIPlan& plan);
    };
} // namespace PJ
