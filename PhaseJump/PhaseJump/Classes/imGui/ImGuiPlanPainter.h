#pragma once

#include "Color.h"
#include "imgui.h"
#include "SomeImGuiPainter.h"
#include "Tags.h"
#include "UIPlan.h"
#include "UnorderedMap.h"

/*
 RATING: 4 stars
 Tested and works. Missing push/pop id
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    /// Paints a UI plan into imGui
    class ImGuiPlanPainter : public SomeImGuiPainter {
    public:
        using DrawModelFunc = std::function<void(This&, SomeUIModel&)>;

        UIPlan& plan;

        UnorderedMap<String, DrawModelFunc> drawModelFuncs;

        ImGuiPlanPainter(UIPlan& plan);
    };
} // namespace PJ
