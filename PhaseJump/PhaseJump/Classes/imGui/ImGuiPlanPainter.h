#pragma once

#include "Color.h"
#include "imgui.h"
#include "UIPlan.h"
#include "UnorderedMap.h"

namespace PJ {
    /// Backing storage for imGui painter
    template <class Key>
    class ImGuiStorage {
    public:
        UnorderedMap<Key, Tags> map;
    };

    /// Paints a UI plan into imGui
    class ImGuiPlanPainter {
    public:
        using ImGuiStorage = PJ::ImGuiStorage<SomeUIModel*>;
        ImGuiStorage& storage;

        using DrawFunc = std::function<void(SomeUIModel&, ImGuiStorage&)>;

        UnorderedMap<String, DrawFunc> drawFuncs;

        ImGuiPlanPainter(ImGuiStorage& storage);

        void Draw(UIPlan& plan);
    };
} // namespace PJ
