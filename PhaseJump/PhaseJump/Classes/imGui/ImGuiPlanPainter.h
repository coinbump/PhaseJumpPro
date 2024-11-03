#pragma once

#include "Color.h"
#include "imgui.h"
#include "SomeImGuiPainter.h"
#include "Tags.h"
#include "UIPlan.h"
#include "UnorderedMap.h"

// /23
namespace PJ {
    /// Paints a UI plan into imGui
    class ImGuiPlanPainter : public SomeImGuiPainter {
    public:
        using Storage = PJ::Storage<String>;
        using DrawModelFunc = std::function<void(This&, SomeUIModel&)>;

        UIPlan& plan;
        Storage& storage;

        UnorderedMap<String, DrawModelFunc> drawModelFuncs;

        ImGuiPlanPainter(UIPlan& plan, Storage& storage);
    };
} // namespace PJ
